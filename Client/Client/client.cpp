#include <boost/lexical_cast.hpp>
#include "client.h"

Client &Client::getInstance()
{
    static Client inst;
    return inst;
}

bool Client::connect(std::string ip, int port, std::string playerName)
{
    using namespace boost::asio;

    try
    {
        ip::tcp::endpoint ep(ip::address::from_string(ip), port);

        _socket.open(ip::tcp::v4());
        _socket.connect(ep);

        _connected = true;
        std::cout << "Connected to: " << ep.address() << ":" << ep.port() << std::endl;

        _socket.write_some(buffer(std::to_string(ClientMessageType::Auth) + " "
                                  + playerName + "\n"));

        read_until(_socket, _buffer, "\n");

        int messageType = 0;
        _is >> messageType;

        if(messageType == ServerMessageType::ClientId)
        {
            _is >> _id;

            _is.ignore(); //skip \n

            std::cout << "Client id: " << _id << std::endl;

            //spawn new thread (main for Client)
            std::thread workerThread([this]{ workerThreadProc(); });
            workerThread.detach();

            return true;
        }
        else
            return false;
    }
    catch(boost::system::system_error& e)
    {
        _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        _socket.close();

        std::cerr << e.what() << std::endl;
        _shouldStop = true;
        return false;
    }

    return false;
}

void Client::workerThreadProc()
{
    using namespace boost::asio;

    try
    {
        //wait for GameStarted message
        read_until(_socket, _buffer, "\n");

        int messageType = 0;
        _is >> messageType;

        if(messageType == ServerMessageType::GameStarted)
        {
            _is >> _opponentName;

            _is >> _cachedPuckPos.x >> _cachedPuckPos.y;

            if(_id == 0) //coords order
            {
                _is >> _cachedPos.x >> _cachedPos.y;
                _is >> _cachedEnemyPos.x >> _cachedEnemyPos.y;
            }
            else
            {
                _is >> _cachedEnemyPos.x >> _cachedEnemyPos.y;
                _is >> _cachedPos.x >> _cachedPos.y;
            }

            _is.ignore(); //skip \n

            _gameStarted = true;

            std::cout << "Game stared!" << std::endl;

            //start listener thread
            std::thread listenerThread(std::bind(&Client::listenerThreadProc, this));
            std::thread senderThread(std::bind(&Client::senderThreadProc, this));

            senderThread.join();

            //stop listener
            _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            _socket.close();

            listenerThread.join();

            std::cout << "All child threads stopped" << std::endl;

            //cleanup

            _id = -1;
            _winnerId = -1;

            _connected = false;
            _shouldStop = false;
            _gameStarted = false;
            _gameOver = false;
        }
    }
    catch(boost::system::system_error& e)
    {
        _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        _socket.close();

        std::cerr << e.what() << std::endl;
        _shouldStop = true;
    }
}

void Client::sendPaddlePos(float x, float y, bool force)
{
    if(!(x == _cachedPos.x && y == _cachedPos.y) || force)
    {
        _cachedPos.x = x;
        _cachedPos.y = y;
        _cachedPos.isReady = true;
    }
}

void Client::getEnemyPaddlePos(float &x, float &y) const
{
    x = _cachedEnemyPos.x;
    y = _cachedEnemyPos.y;
}

void Client::getPaddlePos(float &x, float &y) const
{
    x = _cachedPos.x;
    y = _cachedPos.y;
}

void Client::getPuckPos(float &x, float &y) const
{
    x = _cachedPuckPos.x;
    y = _cachedPuckPos.y;
}

const std::string &Client::getOpponentName() const
{
    return _opponentName;
}

bool Client::getCollision(int &x, int &force)
{
    if(_cachedCollision.isReady)
    {
        x = _cachedCollision.x;
        force = _cachedCollision.y;

        _cachedCollision.isReady = false;

        return true;
    }
    else
        return false;
}

bool Client::getGoal(int &playerId, int &absoluteScore)
{
    if(_cachedGoal.isReady)
    {
        playerId = _cachedGoal.x;
        absoluteScore = _cachedGoal.y;

        _cachedGoal.isReady = false;

        return true;
    }
    else
        return false;
}

bool Client::isGameStarted() const
{
    return _gameStarted;
}

int Client::id() const
{
    return _id;
}

int Client::getWinnerId() const
{
    return _winnerId;
}

bool Client::isConnected() const
{
    return _connected;
}

bool Client::isGameOver() const
{
    return _gameOver;
}

bool Client::shouldStop() const
{
    return _shouldStop;
}

void Client::stop()
{
    _shouldStop = true;
}

Client::Client()
    : _socket(_service), _is(&_buffer)
{

}

void Client::sendPos()
{
    _mutex.lock();

    if(_cachedPos.isReady)
    {
        try
        {
            _socket.send(boost::asio::buffer(
                          std::to_string(ClientMessageType::PaddlePos) + " " +
                          std::to_string(_cachedPos.x) + " " +
                          std::to_string(_cachedPos.y) + "\n"));

            _cachedPos.isReady = false;

#ifdef _DEBUG
            std::cout << "Coords sent" << std::endl;
#endif

        }
        catch(boost::system::system_error& e)
        {
            _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            _socket.close();

            std::cerr << e.what() << std::endl;
            _shouldStop = true;
        }
    }

    _mutex.unlock();
}


void Client::senderThreadProc()
{
    while(!_shouldStop)
    {
        if(_cachedPos.isReady)
        {
            sendPos();
            _cachedPos.isReady = false;
        }

        //relax CPU for a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}


void Client::listenerThreadProc()
{
    while(!_shouldStop)
    {
        try
        {
            read_until(_socket, _buffer, "\n");
        }
        catch(boost::system::system_error& e)
        {
            std::cerr << e.what() << std::endl;
            _shouldStop = true;
        }

        int messageType = 0;
        _is >> messageType;

        switch(messageType)
        {
        case ServerMessageType::PaddlePos:
        {
            int coordX = 0;
            int coordY = 0;

            _is >> coordX >> coordY;
            _is.ignore(); //skip \n

            _mutex.lock();
            //update view

            _cachedEnemyPos.x = coordX;
            _cachedEnemyPos.y = coordY;
            _cachedEnemyPos.isReady = true;

#ifdef _DEBUG
            std::cout << "Coords recieved: " << coordX << " " << coordY << std::endl;
#endif

            _mutex.unlock();

            break;
        }
        case ServerMessageType::PuckPos:
        {
            int coordX = 0;
            int coordY = 0;

            _is >> coordX >> coordY;
            _is.ignore(); //skip \n

            _mutex.lock();

            _cachedPuckPos.x = coordX;
            _cachedPuckPos.y = coordY;

#ifdef _DEBUG
            std::cout << "Puck pos received: " << coordX << " " << coordY << std::endl;
#endif

            _mutex.unlock();

            break;
        }
        case ServerMessageType::Collision:
        {
            int coordX = 0;
            int coordY = 0;

            _is >> coordX >> coordY;
            _is.ignore(); //skip \n

            _mutex.lock();

            _cachedCollision.x = coordX;
            _cachedCollision.y = coordY;
            _cachedCollision.isReady = true;

#ifdef _DEBUG
            std::cout << "Collision received: " << coordX << " " << coordY << std::endl;
#endif

            _mutex.unlock();

            break;
        }
        case ServerMessageType::Goal:
        {
            int playerId = 0;
            int absoluteScore = 0;

            _is >> playerId >> absoluteScore;
            _is.ignore(); //skip \n

            _mutex.lock();

            _cachedGoal.x = playerId;
            _cachedGoal.y = absoluteScore;
            _cachedGoal.isReady = true;

            _mutex.unlock();

            break;
        }
        case ServerMessageType::GameOver:
            std::cout << "GameOver recieved!" << std::endl;

            _is >> _winnerId;
            _is.ignore(); //skip \n

            if(_winnerId == _id)
                std::cout << "Win!" << std::endl;
            else
                std::cout << "Loose!" << std::endl;

            _gameOver = true;

            _shouldStop = true;
            break;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

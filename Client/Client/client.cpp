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

        boost::asio::streambuf buffer;
        boost::system::error_code err;
        read_until(_socket, buffer, "\n", err);

        std::istream is(&buffer);

        read_until(_socket, buffer, "\n");

        int messageType = 0;
        is >> messageType;

        if(messageType == ServerMessageType::ClientId)
        {
            is >> _id;

            is.ignore(); //skip \n

            std::cout << "Client id: " << _id << std::endl;

            //wait for GameStarted message
            read_until(_socket, buffer, "\n");

            int messageType = 0;
            is >> messageType;

            if(messageType == ServerMessageType::GameStarted)
            {
                is >> _cachedPuckPos.x >> _cachedPuckPos.y;

                if(_id == 0) //coords order
                {
                    is >> _cachedPos.x >> _cachedPos.y;
                    is >> _cachedEnemyPos.x >> _cachedEnemyPos.y;
                }
                else
                {
                    is >> _cachedEnemyPos.x >> _cachedEnemyPos.y;
                    is >> _cachedPos.x >> _cachedPos.y;
                }

                is.ignore(); //skip \n

                _gameStarted = true;

                std::cout << "Game stared!" << std::endl;

                //start listener thread
                std::thread listenerThread(std::bind(&Client::listenerThreadProc, this));
                std::thread senderThread(std::bind(&Client::senderThreadProc, this));

                listenerThread.join();
                senderThread.join();
            }
        }


        _socket.shutdown(ip::tcp::socket::shutdown_receive);
        _socket.close();
    }
    catch(boost::system::system_error& e)
    {
        _socket.close();

        std::cerr << e.what() << std::endl;
        _shouldStop = true;

        return false;
    }

    return true;
}

void Client::sendPaddlePos(float x, float y) //REVIEW: -_-
{
    //if(x != _cachedPos.x || y != _cachedPos.y)
    //{
        _cachedPos.x = x;
        _cachedPos.y = y;
        _cachedPos.isReady = true;
    //}
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

bool Client::getCollision(int &x, int &force)
{
    if(_cachedCollision.isReady)
    {
        x = _cachedCollision.x;
        force = _cachedCollision.y;

        _cachedCollision.isReady = false;
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

bool Client::isConnected() const
{
    return _connected;
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
    : _socket(_service)
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
    boost::asio::streambuf buffer;
    std::istream is(&buffer);

    while(!_shouldStop)
    {
        try
        {
            read_until(_socket, buffer, "\n");
        }
        catch(boost::system::system_error& e)
        {
            std::cerr << e.what() << std::endl;
            _shouldStop = true;
        }

        int messageType = 0;
        is >> messageType;

        switch(messageType)
        {
        case ServerMessageType::PaddlePos:
        {
            int coordX = 0;
            int coordY = 0;

            is >> coordX >> coordY;
            is.ignore(); //skip \n

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

            is >> coordX >> coordY;
            is.ignore(); //skip \n

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

            is >> coordX >> coordY;
            is.ignore(); //skip \n

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
        case ServerMessageType::GameOver:
            std::cout << "GameOver recieved!" << std::endl;
            _shouldStop = true;
            break;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

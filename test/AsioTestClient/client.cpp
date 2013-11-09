#include "client.h"

Client &Client::getInstance()
{
    static Client inst;
    return inst;
}

void Client::connect(std::string ip, int port)
{
    using namespace boost::asio;

    try
    {
        ip::tcp::endpoint ep(ip::address::from_string(ip), port);

        _socket.open(ip::tcp::v4());
        _socket.connect(ep);

        std::cout << "Connected to: " << ep.address() << ":" << ep.port() << std::endl;

        _socket.write_some(buffer(std::to_string(ClientMessageType::Auth) + " Vasja\n"));

        boost::asio::streambuf buffer;
        boost::system::error_code err;
        read_until(_socket, buffer, "\n", err);

        std::istream is(&buffer);

        read_until(_socket, buffer, "\n");

        int messageType = 0;
        is >> messageType;

        if(messageType == ServerMessageType::ClientId)
        {
            std::string clientId;
            is >> clientId;

            is.ignore(); //skip \n

            std::cout << "Client id: " << clientId << std::endl;

            //wait for GameStarted message
            read_until(_socket, buffer, "\n");

            int messageType = 0;
            is >> messageType;

            if(messageType == ServerMessageType::GameStarted)
            {
                _gameStarted = true;

                std::cout << "Game stared!" << std::endl;

                //start listener thread
                std::thread listenerThread(std::bind(&Client::listenerThreadProc, this));
                std::thread inputThread(std::bind(&Client::inputThreadProc, this));

                listenerThread.join();
                inputThread.join();
            }
        }


        _socket.shutdown(ip::tcp::socket::shutdown_receive);
        _socket.close();
    }
    catch(std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Unexpected error" << std::endl;
    }
}

void Client::sendPaddleCoords(int x, int y)
{
    _cachedCoords.x = x;
    _cachedCoords.y = y;
    _cachedCoords.isReady = true;
}

void Client::getEnemyPaddleCoords(int &x, int &y)
{
    x = _cachedEnemyCoords.x;
    y = _cachedEnemyCoords.y;
}

bool Client::isGameStarted() const
{
    return _gameStarted;
}

Client::Client()
    : _socket(_service)
{

}

void Client::sendCoords()
{
    _mutex.lock();

    if(_cachedCoords.isReady)
    {
        _socket.send(boost::asio::buffer(
                      std::to_string(ClientMessageType::PaddlePos) + " " +
                      std::to_string(_cachedCoords.x) + " " +
                      std::to_string(_cachedCoords.y) + "\n"));

        _cachedCoords.isReady = false;

        std::cout << "Coords sent" << std::endl;
    }

    _mutex.unlock();
}


void Client::inputThreadProc()
{
    while(!_shouldStop)
    {
        if(_cachedCoords.isReady)
        {
            sendCoords();
            _cachedCoords.isReady = false;
        }

        //relax CPU for a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void Client::listenerThreadProc()
{
    boost::asio::streambuf buffer;
    std::istream is(&buffer);

    while(!_shouldStop)
    {
        read_until(_socket, buffer, "\n");

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

            _cachedEnemyCoords.x = coordX;
            _cachedEnemyCoords.y = coordY;
            _cachedEnemyCoords.isReady = true;

            std::cout << "Coords recieved: " << coordX << " " << coordY << std::endl;
            _mutex.unlock();

            break;
        }
        case ServerMessageType::GameOver:
            std::cout << "GameOver recieved!" << std::endl;
            _shouldStop = true;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

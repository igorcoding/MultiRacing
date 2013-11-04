#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

std::mutex _mutex;

namespace ClientMessageType
{
    enum { Error, Auth, Coord };
}

namespace ServerMessageType
{
    enum { Error, ClientId, GameStarted, BeaterPos, PuckPos, GameOver, Score };
}

void sendData(boost::asio::ip::tcp::socket &sock, int coordX, int coordY)
{
    _mutex.lock();

    sock.send(boost::asio::buffer(
                  std::to_string(ClientMessageType::Coord) + " 30 40\n"));


    _mutex.unlock();
}

void listenerThread(boost::asio::ip::tcp::socket &sock)
{
    boost::asio::streambuf buffer;
    std::istream is(&buffer);
    bool _shouldStop = false;

    while(!_shouldStop)
    {
        _mutex.lock();

        read_until(sock, buffer, "\n");

        int messageType = 0;
        is >> messageType;

        switch(messageType)
        {
        case ServerMessageType::PuckPos:
        {
            int coordX = 0;
            int coordY = 0;

            is >> coordX >> coordY;
            is.ignore(); //skip \n

            std::cout << "Puck pos: " << coordX << " " << coordY << std::endl;
            break;
        }
        case ServerMessageType::GameOver:
            std::cout << "GameOver recieved!" << std::endl;
            _shouldStop = true;
            break;
        }

        _mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main()
{
    using namespace boost::asio;

    try
    {
        io_service service;
        ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 14882);
        ip::tcp::socket sock(service);
        sock.open(ip::tcp::v4());
        sock.connect(ep);

        std::cout << "Connected to: " << ep.address() << ":" << ep.port() << std::endl;

        sock.write_some(buffer(std::to_string(ClientMessageType::Auth) + " Vasja\n"));

        boost::asio::streambuf buffer;
        boost::system::error_code err;
        read_until(sock, buffer, "\n", err);

        std::istream is(&buffer);

        read_until(sock, buffer, "\n");

        int messageType = 0;
        is >> messageType;

        if(messageType == ServerMessageType::ClientId)
        {
            std::string clientId;
            is >> clientId;

            is.ignore(); //skip \n

            std::cout << "Client id: " << clientId << std::endl;

            //wait for GameStarted message
            read_until(sock, buffer, "\n");

            int messageType = 0;
            is >> messageType;

            if(messageType == ServerMessageType::GameStarted)
            {
                std::cout << "Game stared!" << std::endl;

                //start listener thread
                std::thread listener(listenerThread, std::ref(sock));
                std::thread inputThread(sendData, std::ref(sock), 10, 20);

                listener.join();
                inputThread.join();
            }
        }


        sock.shutdown(ip::tcp::socket::shutdown_receive);
        sock.close();
    }
    catch(std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Unexpected error" << std::endl;
    }

    std::cin.peek(); //prevent from Qt
}


#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <sstream>
#include <memory>
#include <functional>
#include <type_traits>
#include "server.h"
#include "logic.h"

Server::Server()
{
}

Server& Server::getInstance()
{
    static Server inst;
    return inst;
}

void Server::start()
{
    try
    {
        using namespace boost::asio::ip;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), _port));
        boost::system::error_code error;

        std::cout << "Server started at port: " << _port << std::endl;

        const int clientsNeeded = 2;

        for(int clientId = 0; clientId < clientsNeeded; )
        {
            //get client
            Client client(io_service);

            acceptor.accept(client.socket, error);

            //get Auth message
            boost::asio::streambuf buffer;
            read_until(client.socket, buffer, "\n", error);

            std::istream is(&buffer);

            int messageType = 0;
            is >> messageType;

            if(messageType == ClientMessageType::Auth)
            {
                //get it's name
                std::string clientName;
                is >> clientName;

                //send it's id
                std::string clientIdStr =
                        std::to_string(ServerMessageType::ClientId) + " " +
                        std::to_string(clientId) + "\n";

                client.socket.send(boost::asio::buffer(clientIdStr));

                std::cout << "Client connected:" << std::endl
                          << " Name: " << clientName << std::endl
                          << " IP: " << client.socket.remote_endpoint().address() << std::endl
                          << " ID: " << clientId << std::endl << std::endl;


                client.id = clientId++;
                clients.emplace_back(std::move(client));
            }
            else
            {
                client.socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
                client.socket.close();
            }
        }

        acceptor.close();

        //let's begin our game

        Logic &logic = Logic::getInstance();

        for(auto &client: clients)
        {
            _cachedPuckPos.x = logic.puck().x;
            _cachedPuckPos.y = logic.puck().y;

            client.socket.send(
                        boost::asio::buffer(
                            std::to_string(ServerMessageType::GameStarted) + " " +
                            std::to_string(_cachedPuckPos.x) + " " +
                            std::to_string(_cachedPuckPos.y) + " " +
                            std::to_string(logic.player(0).x) + " " +
                            std::to_string(logic.player(0).y) + " " +
                            std::to_string(logic.player(1).x) + " " +
                            std::to_string(logic.player(1).y) + "\n"));

            //start listener thread
            client.thread = std::thread(
                        [this, &client](){ listenerThreadProc(client); });
        }

        std::cout << "Game started!" << std::endl;

        //start logic thread
        std::thread logicThread(std::bind(&Logic::start,
                                          std::ref(Logic::getInstance())));


        std::thread senderThread(std::bind(&Server::senderThreadProc,
                                          std::ref(Server::getInstance())));


        senderThread.join();

        logicThread.join();

        switch(Logic::getInstance().reason())
        {
        case Logic::StopReason::ClientDisconnected:
        {
            std::cout << "Client disconnected!" << std::endl;
            break;
        }
        case Logic::StopReason::GameOver:
        {
            //send GameOver to all clients
            for(auto &client: clients)
            {
                client.socket.send(
                            boost::asio::buffer(
                                std::to_string(ServerMessageType::GameOver) + "\n"));
                break;
            }
        }
        case Logic::StopReason::ServerStopped:
        {
            break;
        }
        }


        //wait
        for(auto &client: clients)
            client.thread.join();

        //free clients
        for(auto &client: clients)
        {
            client.socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
            client.socket.close();
        }
    }
    catch(boost::system::system_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Server::setPuckPos(int x, int y)
{
    if(x != _cachedPuckPos.x || y != _cachedPuckPos.y)
    {
        _cachedPuckPos.x = x;
        _cachedPuckPos.y = y;

        _cachedPuckPos.isReady = true;
    }
}

void Server::setCollisionPos(int x, int y)
{
    _cachedCollisionPos.x = x;
    _cachedCollisionPos.y = y;

    _cachedCollisionPos.isReady = true;
}

void Server::listenerThreadProc(Client &client)
{
    boost::asio::streambuf buffer;
    std::istream is(&buffer);

    try
    {
        while(!Logic::getInstance().shouldStop())
        {
            read_until(client.socket, buffer, "\n");

            int messageType = 0;
            is >> messageType;

            //падает тут при чтении данных от второго клиента
            switch(messageType)
            {
            case ClientMessageType::PaddlePos:
            {
                int coordX = 0;
                int coordY = 0;

                is >> coordX >> coordY;
                is.ignore(); //skip \n

#ifdef _DEBUG
                std::cout << "Coords recieved from client: " << client.id << std::endl
                          << "x: " << coordX << " y: " << coordY << std::endl;
#endif

                //does nothing for now
                Logic::getInstance().setPos(client.id, coordX, coordY);
                sendCoords(!client.id, coordX, coordY);

                break;
            }

            default:
                std::cerr << "Unknown message type";
                break;
            }
        }
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        Logic::getInstance().stop(Logic::StopReason::ClientDisconnected);
    }
}

void Server::senderThreadProc()
{
    while(!Logic::getInstance().shouldStop())
    {
        try
        {
            if(_cachedPuckPos.isReady)
            {
                sendPuckPos();
               _cachedPuckPos.isReady = false;
            }

            if(_cachedCollisionPos.isReady)
            {
                sendCollisionPos();
                _cachedCollisionPos.isReady = false;
            }
        }
        catch(std::exception &e)
        {
            Logic::getInstance().stop(Logic::StopReason::ClientDisconnected);
        }

        //relax CPU for a bit
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

void Server::sendCoords(int clientId, int x, int y)
{
    clients[clientId].socket.send(boost::asio::buffer(
                  std::to_string(ServerMessageType::PaddlePos) + " " +
                  std::to_string(x) + " " + std::to_string(y) + "\n"));
}

void Server::sendPuckPos()
{
    for(auto &client: clients)
    {
        client.socket.send(boost::asio::buffer(
                      std::to_string(ServerMessageType::PuckPos) + " " +
                      std::to_string(_cachedPuckPos.x) + " " +
                      std::to_string(_cachedPuckPos.y) + "\n"));
    }
}

void Server::sendCollisionPos()
{
    for(auto &client: clients)
    {
        client.socket.send(boost::asio::buffer(
                      std::to_string(ServerMessageType::Collision) + " " +
                      std::to_string(_cachedCollisionPos.x) + " " +
                      std::to_string(_cachedCollisionPos.y) + "\n"));
    }
}

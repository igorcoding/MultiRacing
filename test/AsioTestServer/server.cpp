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
        for(auto &client: clients)
        {
            client.socket.send(
                        boost::asio::buffer(
                            std::to_string(ServerMessageType::GameStarted) + "\n"));

            //start listener thread
            client.thread = std::thread(
                        [this, &client](){ listenerThread(client); });
        }

        std::cout << "Game started!" << endl;

        //start logic thread
        std::thread logicThread(std::bind(&Logic::start,
                                          std::ref(Logic::getInstance())));

        logicThread.join();

        //send GameOver to all clients
        for(auto &client: clients)
        {
            client.socket.send(
                        boost::asio::buffer(
                            std::to_string(ServerMessageType::GameOver) + "\n"));
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
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Server::listenerThread(Client &client)
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

                std::cout << "Coords recieved from client: " << client.id << std::endl
                          << "x: " << coordX << " y: " << coordY << std::endl;

                //does nothing for now
                Logic::getInstance().processCoords(client.id, coordX, coordY);
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
    }
}

void Server::sendCoords(int clientId, int x, int y)
{
    clients[clientId].socket.send(boost::asio::buffer(
                  std::to_string(ServerMessageType::PaddlePos) + " " +
                  std::to_string(x) + " " +
                  std::to_string(y) + "\n"));
}

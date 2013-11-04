#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <memory>

namespace ClientMessageType
{
    enum { Error, Auth, Coord };
}

namespace ServerMessageType
{
    enum { Error, ClientId, GameStarted, BeaterPos, PuckPos, GameOver, Score };
}

struct Client
{
    Client(boost::asio::io_service &service)
        : socket(service)
    { }

    boost::asio::ip::tcp::socket socket;
    std::thread thread;
    int id;
};

class Server
{
public:
    static Server& getInstance();

    void start();
    static void listenerThread(Client &client);

    void sendData();

private:
    Server();
    Server(const Server& root) = delete;
    Server& operator=(const Server&) = delete;

    std::vector<Client> clients;
    const int _port = 14882;
};

#endif // SERVER_H

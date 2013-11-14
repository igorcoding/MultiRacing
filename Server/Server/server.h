#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <memory>


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
    void listenerThread(Client &client);

    void sendCoords(int clientId, int x, int y);

    void sendPuckPos(int x, int y);

private:
    Server();
    Server(const Server& root) = delete;
    Server& operator=(const Server&) = delete;

    struct ClientMessageType { enum { Error, Auth, PaddlePos }; };
    struct ServerMessageType { enum { Error, ClientId, GameStarted, PaddlePos, PuckPos, GameOver, Score }; };

    boost::asio::io_service io_service;
    std::vector<Client> clients;
    const int _port = 14882;
};

#endif // SERVER_H

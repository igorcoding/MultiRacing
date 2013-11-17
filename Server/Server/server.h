#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <memory>
#include <chrono>

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

    void setPuckPos(int x, int y);

private:
    Server();
    Server(const Server& root) = delete;
    Server& operator=(const Server&) = delete;

    struct Data
    {
        int x = 0, y = 0;
        bool isReady = false;
    } _cachedPuckPos;

    void listenerThreadProc(Client &client);

    //sends puck pos for all clients
    void senderThreadProc();

    void sendCoords(int clientId, int x, int y);

    void sendPuckPos();


    struct ClientMessageType { enum { Error, Auth, PaddlePos }; };
    struct ServerMessageType { enum { Error, ClientId, GameStarted, PaddlePos, PuckPos, GameOver, Score }; };

    boost::asio::io_service io_service;
    std::vector<Client> clients;
    const int _port = 14882;

    std::chrono::microseconds _senderPeriod = std::chrono::milliseconds(30);
    std::chrono::microseconds _listenerPeriod = std::chrono::microseconds(1);
};

#endif // SERVER_H

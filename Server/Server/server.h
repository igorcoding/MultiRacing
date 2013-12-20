#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <memory>
#include <chrono>
#include <future>

struct Client
{
    Client(boost::asio::io_service &service)
        : socket(service)
    { }

    boost::asio::ip::tcp::socket socket;
    std::thread thread;
    std::string name;
    int id;
};

class Server
{
public:
    static Server& getInstance();

    bool start(int port);
    void stop();

    //TODO: maybe make universal setter/getter/sender for all the packets?
    void setPuckPos(int x, int y);
    void setCollision(int x, int force);
    void setGoal(int playerId, int absoluteScore);
    void setWinner(int playerId);

private:
    Server();
    Server(const Server& root) = delete;
    Server& operator=(const Server&) = delete;

    struct Data
    {
        int x = 0, y = 0;
        bool isReady = false;
    } _cachedPuckPos, _cachedCollision, _cachedGoal, _cachedWinner;

    void workerThreadProc(boost::asio::ip::tcp::acceptor &&acceptor);
    void listenerThreadProc(Client &client);
    void senderThreadProc();

    void sendCoords(int clientId, int x, int y);

    void sendPuckPos();
    void sendCollisionPos();
    void sendGoal();
    void sendGameOver();


    //TODO: move them to some kind of shared (between Client and Server) config file
    struct ClientMessageType { enum { Error, Auth, PaddlePos }; };
    struct ServerMessageType { enum { Error, ClientId, GameStarted, PaddlePos, PuckPos, Collision, Goal, GameOver, Score }; };

    boost::asio::io_service io_service;
    std::vector<Client> clients;
    std::future<void> _workerFuture;

    std::chrono::microseconds _senderPeriod = std::chrono::milliseconds(30);
    std::chrono::microseconds _listenerPeriod = std::chrono::microseconds(1);
};

#endif // SERVER_H

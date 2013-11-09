#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace ClientMessageType
{
    enum { Error, Auth, PaddlePos };
}

namespace ServerMessageType
{
    enum { Error, ClientId, GameStarted, PaddlePos, PuckPos, GameOver, Score };
}

class Client
{
public:
    static Client& getInstance();

    void connect(std::string ip, int port);

private:
    Client();
    Client(const Client& root) = delete;
    Client& operator=(const Client&) = delete;

    void sendCoords(int x, int y);

    void inputThreadProc();
    void listenerThreadProc();

    boost::asio::io_service _service;

    std::mutex _mutex;
    boost::asio::ip::tcp::socket _socket;

    bool _shouldStop = false;
};

#endif // CLIENT_H

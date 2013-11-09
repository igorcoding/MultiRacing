#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

class Client
{
public:
    static Client& getInstance();

    //connects to the server
    void connect(std::string ip, int port);

    //sets Data's values and isReady flag
    void sendPaddleCoords(int x, int y);

    //immediately returns cached coords
    void getEnemyPaddleCoords(int &x, int &y);

    bool isGameStarted() const;

private:
    Client();
    Client(const Client& root) = delete;
    Client& operator=(const Client&) = delete;


    struct ClientMessageType { enum { Error, Auth, PaddlePos }; };
    struct ServerMessageType { enum { Error, ClientId, GameStarted, PaddlePos, PuckPos, GameOver, Score }; };

    struct Data
    {
        int x = 0, y = 0;
        bool isReady = false;
    } _cachedCoords, _cachedEnemyCoords;

    //sends _data's values
    void sendCoords();

    void inputThreadProc();
    void listenerThreadProc();

    boost::asio::io_service _service;

    std::mutex _mutex;
    boost::asio::ip::tcp::socket _socket;

    bool _shouldStop = false;
    bool _gameStarted = false;
};

#endif // CLIENT_H

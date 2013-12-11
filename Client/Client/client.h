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
    bool connect(std::string ip, int port, std::string playerName);

    //sets Data's values and isReady flag
    void sendPaddlePos(float x, float y);

    //immediately returns cached coords
    void getEnemyPaddlePos(float &x, float &y) const;

    //get self coords
    void getPaddlePos(float &x, float &y) const;

    void getPuckPos(float &x, float &y) const;

    //test if collision occured,
    //if it is => sets x and force
    //and resets internal flag, so next time it will not return true
    //(until new collision will be occured)
    bool getCollision(int &x, int &force);

    //returns true after GameStarted message from server
    bool isGameStarted() const;

    //gets client id (0 or 1, or -1 if id isn't assigned yet)
    int id() const;

    bool isConnected() const;

    //if any errors occured
    bool shouldStop() const;

    void stop();

private:
    Client();
    Client(const Client& root) = delete;
    Client& operator=(const Client&) = delete;


    struct ClientMessageType { enum { Error, Auth, PaddlePos }; };
    struct ServerMessageType { enum { Error, ClientId, GameStarted, PaddlePos, PuckPos, Collision, GameOver, Score }; };

    struct Data
    {
        int x = 0, y = 0;
        bool isReady = false;
    } _cachedPos, _cachedEnemyPos, _cachedPuckPos, _cachedCollision;

    //_cachedPuckPos.isReady flag is not used anywhere


    //sends _data's values
    void sendPos();

    void senderThreadProc();
    void listenerThreadProc();
    void workerThreadProc();

    boost::asio::io_service _service;

    std::mutex _mutex;
    boost::asio::ip::tcp::socket _socket;

    boost::asio::streambuf _buffer;
    std::istream _is;

    int _id = -1;
    bool _connected = false;
    bool _shouldStop = false;
    bool _gameStarted = false;
};

#endif // CLIENT_H

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

    const std::string &getOpponentName() const;

    //test if collision occured,
    //if it is => sets x and force
    //and resets internal flag, so next time it will not return true
    //(until new collision will be occured)
    bool getCollision(int &x, int &force);

    //test if goal occured,
    //if it is => sets playerId and his absoluteScore
    //and resets internal flag, so next time it will not return true
    //(until new goal will be occured)
    bool getGoal(int &playerId, int &absoluteScore);

    //returns true after GameStarted message from server
    bool isGameStarted() const;

    //gets client id (0 or 1, or -1 if id isn't assigned yet)
    int id() const;

    int getWinnerId() const;

    bool isConnected() const;

    bool isGameOver() const;

    //if any errors occured
    bool shouldStop() const;

    void stop();

private:
    Client();
    Client(const Client& root) = delete;
    Client& operator=(const Client&) = delete;


    struct ClientMessageType { enum { Error, Auth, PaddlePos }; };
    struct ServerMessageType { enum { Error, ClientId, GameStarted, PaddlePos, PuckPos, Collision, Goal, GameOver, Score }; };

    struct Data
    {
        int x = 0, y = 0;
        bool isReady = false;
    } _cachedPos, _cachedEnemyPos, _cachedPuckPos, _cachedCollision, _cachedGoal;
    //_cachedPuckPos.isReady flag is not used anywhere

    std::string _opponentName;

    //sends _data's values
    void sendPos();

    void senderThreadProc();
    void listenerThreadProc();

    boost::asio::io_service _service;

    std::mutex _mutex;
    boost::asio::ip::tcp::socket _socket;

    int _id = -1;
    int _winnerId = -1;

    bool _connected = false;
    bool _shouldStop = false;
    bool _gameStarted = false;
    bool _gameOver = false;
};

#endif // CLIENT_H

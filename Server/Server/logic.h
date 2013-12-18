#ifndef LOGIC_H
#define LOGIC_H
#include <mutex>
#include <chrono>
#include <vector>

#include "player.h"
#include "puck.h"

class Logic
{
public:
    static Logic &getInstance();

    enum class StopReason { ClientDisconnected, GameOver, ServerStopped, LogicException };

    void start();

    void setPos(int clientId, int x, int y);
    const Player& player(int id);
    const Puck& puck();

    void stop(StopReason reason);

    bool shouldStop() const;
    StopReason reason() const;

    int getWinnerId() const;

    const static int defaultScreenWidth = 800;
    const static int defaultScreenHeight = 600;
private:
    Logic();
    Logic(const Logic& root) = delete;
    Logic& operator=(const Logic&) = delete;

    void setInitialCoords();
    bool frameFunc(double dt);
    void checkCollisions();
    void handleCollision(Player& p);
    void handleWallCollisionsAndGoal();
    bool checkGoal();
    void handleGoal(int goaler);


    std::vector<Player> _players;
    Puck _puck;
    bool _initialized;

    int _winnerId = -1;
    int _scoresToWin = 7;

    std::chrono::milliseconds _framePeriod = std::chrono::milliseconds(10);
    std::mutex _mutex;
    bool _shouldStop = false;
    StopReason _reason;
};

#endif // LOGIC_H

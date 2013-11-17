#include "logic.h"
#include "server.h"

Logic::Logic()
    : _initialized(false)
{
    _players.emplace_back(std::move(Player(0)));
    _players.emplace_back(std::move(Player(1)));
}

Logic& Logic::getInstance()
{
    static Logic inst;
    if (!inst._initialized)
        inst.setInitialCoords();
    return inst;
}

void Logic::start()
{
    using namespace std::chrono;

    time_point<system_clock> lastTime = system_clock::now();

    do
    {
        _mutex.lock();

        std::chrono::duration<double> dt = system_clock::now() - lastTime;
        lastTime = system_clock::now();

        _shouldStop = frameFunc(dt.count());

        _mutex.unlock();

        std::this_thread::sleep_for(_framePeriod);
    }
    while(!_shouldStop);
}

void Logic::setPos(int clientId, int x, int y)
{
    //save coords to underlying structures
    _players[clientId].x = x;
    _players[clientId].y = y;
}

const Player& Logic::player(int id)
{
    return _players[id];
}

const Puck& Logic::puck()
{
    return _puck;
}

void Logic::stop(Logic::StopReason reason)
{
    _reason = reason;
    _shouldStop = true;
}

bool Logic::frameFunc(double dt)
{
    //being called each dt seconds

    //send changes to all clients

    //testing
    /*static int x = 0;
    static int y = 200;

    x = (x + 5)%800;

    Server::getInstance().setPuckPos(x, y);
*/
    return false;
}

void Logic::setInitialCoords()
{
    // initial rendering values
    const float x_offset = 50.0f;
    _puck.x = defaultScreenWidth / 2;
    _puck.y = defaultScreenHeight / 2;
    for (size_t i = 0; i < _players.size(); ++i)
    {
        _players[i].x = fabs((int) _players[i].side * defaultScreenWidth - x_offset);
        _players[i].y = defaultScreenHeight / 2;
    }
    _initialized = true;
}

bool Logic::shouldStop() const
{
    return _shouldStop;
}

Logic::StopReason Logic::reason() const
{
    return _reason;
}

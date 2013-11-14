#include "logic.h"
#include "server.h"

Logic::Logic()
{
}

Logic& Logic::getInstance()
{
    static Logic inst;
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
    static int x = 0;
    static int y = 200;

    x = (x + 5)%800;

    Server::getInstance().setPuckPos(x, y);

    return false;
}

bool Logic::shouldStop() const
{
    return _shouldStop;
}

Logic::StopReason Logic::reason() const
{
    return _reason;
}

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

void Logic::setCoords(int clientId, int coordX, int coordY)
{
    _mutex.lock();

    //save player's coords


    _mutex.unlock();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(_framePeriod));
    }
    while(!_shouldStop);
}

bool Logic::frameFunc(double dt)
{
    //being called each dt seconds

    //send changes to all clients
    //Server::getInstance().sendData();

    return false;
}

bool Logic::shouldStop() const
{
    return _shouldStop;
}

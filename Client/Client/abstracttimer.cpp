#include "abstracttimer.h"


AbstractTimer::AbstractTimer(float limit, bool startNow, std::function<void (float)> handler)
{
    _handler = handler;
    _started = startNow;
    _limit = limit;
}

void AbstractTimer::setHandler(std::function<void(float)> handler)
{
    _handler = handler;
}

void AbstractTimer::setLimit(float limit)
{
    _limit = limit;
}

void AbstractTimer::start(float limit)
{
    _started = true;
    _limit = limit;
    _elapsed = 0;
}


void AbstractTimer::stop()
{
    _started = false;
}


bool AbstractTimer::isStarted() const
{
    return _started;
}

float AbstractTimer::elapsed() const
{
    return _elapsed;
}


#include "delaytimer.h"

void DelayTimer::update(float dt)
{
    _elapsed += dt;

    if(_elapsed >= _limit)
    {
        _handler(dt);
        _started = false;
    }
}

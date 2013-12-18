#include "delaytimer.h"

void DelayTimer::update(float dt)
{
    if(_started)
    {
        _elapsed += dt;

        if(_elapsed >= _limit)
        {
            _handler(this, dt);
            _started = false;
        }
    }
}

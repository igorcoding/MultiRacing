#include "untiltimer.h"

void UntilTimer::update(float dt)
{
    if(_started)
    {
        _elapsed += dt;

        if(_elapsed <= _limit)
            _handler(dt);
        else
            _started = false;
    }
}

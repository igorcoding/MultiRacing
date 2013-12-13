#ifndef DELAYTIMER_H
#define DELAYTIMER_H
#include "abstracttimer.h"

class DelayTimer : public AbstractTimer
{
public:
    using AbstractTimer::AbstractTimer; //inherit c-tors

    void update(float dt);
};

#endif // DELAYTIMER_H

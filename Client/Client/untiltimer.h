#ifndef UNTILTIMER_H
#define UNTILTIMER_H

#include "abstracttimer.h"

class UntilTimer : public AbstractTimer
{
public:
    using AbstractTimer::AbstractTimer; //inherit c-tors

    void update(float dt);
};

#endif // UNTILTIMER_H

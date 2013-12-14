#ifndef TIMERFACTORY_H
#define TIMERFACTORY_H
#include <vector>
#include "abstracttimer.h"


class TimerFactory
{
public:
    enum class InvokeType { OnRender, OnFrame };

    void createUntilTimer(InvokeType invokeType, float limit, bool startNow, const std::function<void (float)> &handler);
    void createDelayTimer(InvokeType invokeType, float limit, bool startNow, const std::function<void (float)> &handler);

    void update(float dt);
    void render(float dt);

private:
    std::vector<AbstractTimer *> renderTimers;
    std::vector<AbstractTimer *> updateTimers;
};

#endif // TIMERFACTORY_H

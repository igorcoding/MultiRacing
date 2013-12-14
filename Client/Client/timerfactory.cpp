#include "timerfactory.h"
#include "untiltimer.h"
#include "delaytimer.h"

void TimerFactory::createUntilTimer(TimerFactory::InvokeType invokeType, float limit, bool startNow, const std::function<void (float)> &handler)
{
    if(invokeType == InvokeType::OnRender)
        renderTimers.push_back(new UntilTimer(limit, startNow, handler));
    else
        updateTimers.push_back(new UntilTimer(limit, startNow, handler));
}

void TimerFactory::createDelayTimer(TimerFactory::InvokeType invokeType, float limit, bool startNow, const std::function<void (float)> &handler)
{
    if(invokeType == InvokeType::OnRender)
        renderTimers.push_back(new DelayTimer(limit, startNow, handler));
    else
        updateTimers.push_back(new DelayTimer(limit, startNow, handler));
}

void TimerFactory::update(float dt)
{
    for(auto it = updateTimers.begin(); it != updateTimers.end(); )
    {
        if((*it)->isStarted())
        {
            (*it)->update(dt);
            ++it;
        }
        else
        {
            delete *it;
            updateTimers.erase(it);
        }
    }
}

void TimerFactory::render(float dt)
{
    for(auto it = renderTimers.begin(); it != renderTimers.end(); )
    {
        if((*it)->isStarted())
        {
            (*it)->update(dt);
            ++it;
        }
        else
        {
            delete *it;
            renderTimers.erase(it);
        }
    }
}

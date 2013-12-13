#ifndef TIMER_H
#define TIMER_H
#include <functional>

class AbstractTimer
{
public:
    AbstractTimer() = default;
    AbstractTimer(float limit, bool startNow, std::function<void(float)> handler);
    virtual ~AbstractTimer() = default;

    virtual void setHandler(std::function<void(float)> handler);
    virtual void setLimit(float limit);
    virtual void start(float limit);
    virtual void stop();
    virtual bool isStarted() const;

    virtual void update(float dt) = 0;

protected:
    std::function<void(float)> _handler;
    float _elapsed = 0;
    float _limit = 0;
    bool _started = false;
};

#endif // TIMER_H

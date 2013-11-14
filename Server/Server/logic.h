#ifndef LOGIC_H
#define LOGIC_H
#include <mutex>

class Logic
{
public:
    static Logic &getInstance();

    enum class StopReason { ClientDisconnected, GameOver, ServerStopped };

    void start();

    void setPos(int clientId, int x, int y);

    void stop(StopReason reason);

    bool shouldStop() const;
    StopReason reason() const;

private:
    Logic();
    Logic(const Logic& root) = delete;
    Logic& operator=(const Logic&) = delete;

    bool frameFunc(double dt);

    int _framePeriod = 5;
    std::mutex _mutex;
    bool _shouldStop = false;
    StopReason _reason;
};

#endif // LOGIC_H

#ifndef LOGIC_H
#define LOGIC_H
#include <mutex>
#include <chrono>

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

    std::chrono::milliseconds _framePeriod = std::chrono::milliseconds(10);
    std::mutex _mutex;
    bool _shouldStop = false;
    StopReason _reason;
};

#endif // LOGIC_H

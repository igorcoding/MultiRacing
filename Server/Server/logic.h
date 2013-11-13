#ifndef LOGIC_H
#define LOGIC_H
#include <mutex>

class Logic
{
public:
    static Logic &getInstance();

    enum class StopReason { ClientDisconnected, GameOver, ServerStopped };

    void setCoords(int clientId, int coordX, int coordY);
    void start();

    void stop(StopReason reason);

    bool shouldStop() const;
    StopReason reason() const;

private:
    Logic();
    Logic(const Logic& root) = delete;
    Logic& operator=(const Logic&) = delete;

    bool frameFunc(double dt);

    int _framePeriod = 1;
    std::mutex _mutex;
    bool _shouldStop = false;
    StopReason _reason;
};

#endif // LOGIC_H

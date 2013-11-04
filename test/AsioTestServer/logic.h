#ifndef LOGIC_H
#define LOGIC_H
#include <mutex>

class Logic
{
public:
    static Logic &getInstance();

    void processCoords(int clientId, int coordX, int coordY);
    void start();
    bool frameFunc();

    bool shouldStop() const;

private:
    Logic();
    Logic(const Logic& root) = delete;
    Logic& operator=(const Logic&) = delete;

    int _framePeriod = 1;
    std::mutex _mutex;
    bool _shouldStop = false;
};

#endif // LOGIC_H

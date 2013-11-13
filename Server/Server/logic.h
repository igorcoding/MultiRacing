#ifndef LOGIC_H
#define LOGIC_H
#include <mutex>

class Logic
{
public:
    static Logic &getInstance();

    void setCoords(int clientId, int coordX, int coordY);
    void start();

    bool shouldStop() const;

private:
    Logic();
    Logic(const Logic& root) = delete;
    Logic& operator=(const Logic&) = delete;

    bool frameFunc(double dt);

    int _framePeriod = 1;
    std::mutex _mutex;
    bool _shouldStop = false;
};

#endif // LOGIC_H

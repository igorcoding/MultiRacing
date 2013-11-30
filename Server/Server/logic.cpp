#include "logic.h"
#include "server.h"

Logic::Logic()
    : _initialized(false)
{
    _players.emplace_back(std::move(Player(0)));
    _players.emplace_back(std::move(Player(1)));
}

Logic& Logic::getInstance()
{
    static Logic inst;
    if (!inst._initialized)
        inst.setInitialCoords();
    return inst;
}

void Logic::start()
{
    using namespace std::chrono;

    time_point<system_clock> lastTime = system_clock::now();

    do
    {
        _mutex.lock();

        std::chrono::duration<double> dt = system_clock::now() - lastTime;
        lastTime = system_clock::now();

        _shouldStop = frameFunc(dt.count());

        _mutex.unlock();

        std::this_thread::sleep_for(_framePeriod);
    }
    while(!_shouldStop);
}

void Logic::setPos(int clientId, int x, int y)
{
    //save coords to underlying structures
    _players[clientId].dx = x - _players[clientId].x;
    _players[clientId].dy = y - _players[clientId].y;
    _players[clientId].x = x;
    _players[clientId].y = y;


}

const Player& Logic::player(int id)
{
    return _players[id];
}

const Puck& Logic::puck()
{
    return _puck;
}

void Logic::stop(Logic::StopReason reason)
{
    _reason = reason;
    _shouldStop = true;
}

bool Logic::frameFunc(double dt)
{
    //being called each dt seconds

    //send changes to all clients

    //testing
    /*static int x = 0;
    static int y = 200;

    x = (x + 5)%800;

    Server::getInstance().setPuckPos(x, y);
*/
    for (auto& p : _players)
    {
        p.vx = p.dx / dt;
        p.vy = p.dy / dt;
    }


/*
    // walls collisions
    auto d = distance(_puck.x, _puck.y, _puck.x, 0);
    if (d < _puck.radius || _puck.y < 0)
    {
        _puck.vy = -_puck.vy;
        _puck.y += _puck.y >= 0 ? _puck.radius - d : _puck.radius + d;
    }
    d = distance(_puck.x, _puck.y, _puck.x, defaultScreenHeight);
    if (d < _puck.radius || _puck.y >= defaultScreenHeight)
    {
        _puck.vy = -_puck.vy;
        _puck.y -= _puck.y < defaultScreenHeight ? _puck.radius - d : _puck.radius + d;
    }
    d = distance(_puck.x, _puck.y, 0, _puck.y);
    if (d < _puck.radius || _puck.x < 0)
    {
        _puck.vx = -_puck.vx;
        _puck.x += _puck.x >= 0 ? _puck.radius - d : _puck.radius + d;
    }
    d = distance(_puck.x, _puck.y, defaultScreenWidth, _puck.y);
    if (d < _puck.radius || _puck.x >= defaultScreenWidth)
    {
        _puck.vx = -_puck.vx;
        _puck.x -= _puck.x < defaultScreenWidth ? _puck.radius - d : _puck.radius + d;
    }
    */

    //collisions with players
    Player& firstPlayer = _players[0];
    Player& secondPlayer = _players[1];

    bool collided_first = distance(firstPlayer.x, firstPlayer.y, _puck.x, _puck.y) < firstPlayer.radius + _puck.radius;
    bool collided_second = distance(secondPlayer.x, secondPlayer.y, _puck.x, _puck.y) < secondPlayer.radius + _puck.radius;

    if (collided_first)
    {
        std::cout << "collided first\n";
        handleCollision(firstPlayer, dt);
    }
    if (collided_second)
    {
        std::cout << "collided second\n";
        handleCollision(secondPlayer, dt);
    }

    const float friction = 0.5f;
    _puck.dx *= friction;
    _puck.dy *= friction;
    _puck.x += _puck.dx;
    _puck.y += _puck.dy;

    auto x_max = defaultScreenWidth - _puck.radius / 2;
    auto x_min = _puck.radius / 2;
    auto y_max = defaultScreenHeight - _puck.radius/ 2;
    auto y_min = _puck.radius / 2;

    if (_puck.x > x_max) {
        _puck.x = x_max - (_puck.x - x_max);
        _puck.dx = -_puck.dx;
    }
    if (_puck.x < x_min) {
        _puck.x = x_min + x_min - _puck.x;
        _puck.dx = -_puck.dx;
    }
    if (_puck.y > y_max) {
        _puck.y = y_max - (_puck.y - y_max);
        _puck.dy = -_puck.dy;
    }
    if (_puck.y < y_min) {
        _puck.y = y_min + y_min - _puck.y;
        _puck.dy = -_puck.dy;
    }




    /*float a = -10;
    _puck.x += _puck.vx * dt;
    _puck.y += _puck.vy * dt;
    _puck.vx += a * dt;
    _puck.vy += a * dt;*/

    Server::getInstance().setPuckPos(_puck.x, _puck.y);

    return false;
}

void Logic::setInitialCoords()
{
    // initial rendering values
    const float x_offset = 80.0f;
    _puck.x = defaultScreenWidth / 2 - 150;
    _puck.y = defaultScreenHeight / 2;
    for (size_t i = 0; i < _players.size(); ++i)
    {
        _players[i].x = fabs((int) _players[i].side * defaultScreenWidth - x_offset);
        _players[i].y = defaultScreenHeight / 2;
    }
    _initialized = true;
}

float Logic::distance(int x1, int y1, int x2, int y2)
{
    auto dx = x1 - x2;
    auto dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}

void Logic::handleCollision(Player& p, double dt)
{
    // разнести объекты

    /*int k = 3; // M/m
    auto p_v_sqr = p.vx * p.vx + p.vy + p.vy;
    auto old_puck_v_sqr = _puck.vx * _puck.vx + _puck.vy + _puck.vy;
    auto puck_v = std::sqrt(k * p_v_sqr + old_puck_v_sqr);

    auto cos_gamma = (k * p.vx + _puck.vx) / puck_v;
    auto sin_gamma = (k * p.vy + _puck.vy) / puck_v;

    _puck.vx = puck_v * cos_gamma;
    _puck.vy = puck_v * sin_gamma;
*/
    const float speed = 10.0f;


    if (p.x < _puck.x)
        _puck.dx += speed*dt;
    if (p.x > _puck.x)
        _puck.dx -= speed*dt;
    if (p.y < _puck.y)
        _puck.dy += speed*dt;
    if (p.y > _puck.y)
        _puck.dy -= speed*dt;


}

bool Logic::shouldStop() const
{
    return _shouldStop;
}

Logic::StopReason Logic::reason() const
{
    return _reason;
}

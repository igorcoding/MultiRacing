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

        try
        {
            _shouldStop = frameFunc(dt.count());
        }
        catch(std::exception &e)
        {
            stop(StopReason::LogicException);
        }

        _mutex.unlock();

        std::this_thread::sleep_for(_framePeriod);
    }
    while(!_shouldStop);
}

void Logic::setPos(int clientId, int x, int y)
{
    Player& p = _players[clientId];
    p.setPos(x, y);
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
    for (auto& p : _players)
    {
        p.setSpeed(p.dx() / dt, p.dy() / dt);
    }

    //collisions with players
    Player& firstPlayer = _players[0];
    Player& secondPlayer = _players[1];

    float d1 = distance(firstPlayer.pos.x(),
                        firstPlayer.pos.y(),
                        _puck.pos.x(),
                        _puck.pos.y());
    float d2 = distance(secondPlayer.pos.x(),
                        secondPlayer.pos.y(),
                        _puck.pos.x(),
                        _puck.pos.y());
    bool collided_first = d1 < firstPlayer.radius + _puck.radius;
    bool collided_second = d2 < secondPlayer.radius + _puck.radius;

    if (collided_first)
    {
        std::cout << "collided first\n";
        handleCollision(firstPlayer, d1, dt);
    }
    if (collided_second)
    {
        std::cout << "collided second\n";
        handleCollision(secondPlayer, d2, dt);
    }


    //float speed = 3.0f;
    float friction = 1.0f;
    auto newPuckPos = _puck.pos + _puck.speed;
    //speed *= friction;
    auto normalizedPuckSpeed = math::normalize(puck().speed);
    auto newPuckSpeed =  _puck.speed + math::Vector2D(-friction * normalizedPuckSpeed.x(), -friction * normalizedPuckSpeed.y());
    _puck.pos = newPuckPos;
    _puck.speed = newPuckSpeed;



    int lr_border = 40;
    int tb_border = 40;
    auto x_max = defaultScreenWidth - lr_border - _puck.radius / 2;
    auto x_min = _puck.radius / 2 + lr_border;
    auto y_max = defaultScreenHeight - tb_border - _puck.radius/ 2;
    auto y_min = _puck.radius / 2 + tb_border;


    if (_puck.pos.x() > x_max) {
        std::cout << "x > max\n";
        _puck.pos.x(x_max - (_puck.pos.x() - x_max));
        _puck.speed.x(-_puck.speed.x());
    }
    if (_puck.pos.x() < x_min) {
        std::cout << "x < min\n";
        _puck.pos.x(x_min + x_min - _puck.pos.x());
        _puck.speed.x(-_puck.speed.x());
    }
    if (_puck.pos.y() > y_max) {
        std::cout << "y > max\n";
        _puck.pos.y(y_max - (_puck.pos.y() - y_max));
        _puck.speed.y(-_puck.speed.y());
    }
    if (_puck.pos.y() < y_min) {
        std::cout << "y < min\n";
        _puck.pos.y(y_min + y_min - _puck.pos.y());
        _puck.speed.y(-_puck.speed.y());
    }


    Server::getInstance().setPuckPos(_puck.pos.x(), _puck.pos.y());

    return false;
}

void Logic::setInitialCoords()
{
    // initial rendering values
    const float x_offset = 80.0f;

    auto puck_x = defaultScreenWidth / 2 - 150;
    auto puck_y = defaultScreenHeight / 2;
    _puck.setPos(puck_x, puck_y);
    _puck.setOldPos(puck_x, puck_y);

    for (size_t i = 0; i < _players.size(); ++i)
    {
        auto player_x = fabs((int) _players[i].side * defaultScreenWidth - x_offset);
        auto player_y = defaultScreenHeight / 2;
        _players[i].setOldPos(player_x, player_y);
        _players[i].setPos(player_x, player_y);
    }
    _initialized = true;
}

float Logic::distance(int x1, int y1, int x2, int y2)
{
    auto dx = x1 - x2;
    auto dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}

void Logic::handleCollision(Player& p, float d, double dt)
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

    /*
    const float speed = 10.0f;


    if (p.x < _puck.x)
        _puck.dx += speed*dt;
    if (p.x > _puck.x)
        _puck.dx -= speed*dt;
    if (p.y < _puck.y)
        _puck.dy += speed*dt;
    if (p.y > _puck.y)
        _puck.dy -= speed*dt;
*/

    //float delta = _puck.radius + p.radius - d + 5;
    //boost::numeric::ublas::vector<float> newPos = delta * normalize(_puck.getPosVector() - p.getPosVector());
    //newPos += _puck.getPosVector();
    //_puck.setPos(newPos[0], newPos[1]);

    float delta = _puck.radius + p.radius - d;
    auto moving = delta * math::normalize(p.speed);
    _puck.pos += moving;


    auto speedLimit = 100.0;
    _puck.speed = (1.0/80.0) * p.speed;
    Server::getInstance().setCollision(_puck.pos.x(), _puck.speed.length()); //at, volume
}

bool Logic::shouldStop() const
{
    return _shouldStop;
}

Logic::StopReason Logic::reason() const
{
    return _reason;
}

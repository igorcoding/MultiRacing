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
    p.setOldPos(p.getPos());
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


boost::numeric::ublas::vector<float> normalize(const boost::numeric::ublas::vector<float>& vec)
{
    boost::numeric::ublas::vector<float> newVec(vec.size());
    auto length = boost::numeric::ublas::norm_2(vec);
    if (length != 0)
    {
        for (auto i = 0; i < vec.size(); ++i)
            newVec[i] = vec[i] / length;
    }
    return newVec;
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

    bool collided_first = distance(firstPlayer.getPos().x,
                                   firstPlayer.getPos().y,
                                   _puck.getPos().x,
                                   _puck.getPos().y) < firstPlayer.radius + _puck.radius;
    bool collided_second = distance(secondPlayer.getPos().x,
                                    secondPlayer.getPos().y,
                                    _puck.getPos().x,
                                    _puck.getPos().y) < secondPlayer.radius + _puck.radius;

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

    using namespace boost::numeric;

    float speed = 10.0f;
    auto normz = normalize(_puck.getSpeedVector());
    ublas::vector<float> newPuckPos = _puck.getPosVector() + normz;
    ublas::vector<float> newPuckSpeed = _puck.getSpeedVector(); //+ _puck.getAccelVector();
    _puck.setPos(newPuckPos[0], newPuckPos[1]);
    _puck.setSpeed(newPuckSpeed[0], newPuckSpeed[1]);


/*
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
*/

    Server::getInstance().setPuckPos(_puck.getPos().x, _puck.getPos().y);

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

    _puck.setSpeed(p.getSpeed());
    Server::getInstance().setCollisionPos(_puck.x, _puck.y);
}

bool Logic::shouldStop() const
{
    return _shouldStop;
}

Logic::StopReason Logic::reason() const
{
    return _reason;
}

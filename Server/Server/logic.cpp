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

    std::cout << "Logic::stop(" << (int)_reason << ") called" << std::endl;
}

void Logic::setInitialCoords()
{
    // initial rendering values
    const float x_offset = 80.0f;

    _puck.reset();
    auto puck_x = defaultScreenWidth / 2;
    auto puck_y = defaultScreenHeight / 2;
    _puck.setPos(puck_x, puck_y);
    _puck.setOldPos(puck_x, puck_y);
    //_puck.setSpeed(-1, 0);

    for (size_t i = 0; i < _players.size(); ++i)
    {
        _players[i].reset();
        auto player_x = fabs((int) _players[i].side * defaultScreenWidth - x_offset);
        auto player_y = defaultScreenHeight / 2;
        _players[i].setPos(player_x, player_y);
        _players[i].setOldPos(player_x, player_y);
    }
    _initialized = true;
}

bool Logic::frameFunc(double dt)
{
    for (auto& p : _players)
        p.setSpeed(p.dx() / dt, p.dy() / dt);

    _puck.setOldPos();
    checkCollisions();

    float friction = 1.0f;
    _puck.pos += _puck.speed;
    auto normalizedPuckSpeed = math::normalize(puck().speed);
    //_puck.speed += math::Vector2D(-friction * normalizedPuckSpeed.x(), -friction * normalizedPuckSpeed.y());

    handleWallCollisionsAndGoal();

    Server::getInstance().setPuckPos(_puck.pos.x(), _puck.pos.y());
    return false;
}

void Logic::checkCollisions()
{
    Player& firstPlayer = _players[0];
    Player& secondPlayer = _players[1];

    float d1 = math::distance(firstPlayer.pos, _puck.pos);
    float d2 = math::distance(secondPlayer.pos, _puck.pos);
    bool collided_first = d1 < firstPlayer.radius + _puck.radius;
    bool collided_second = d2 < secondPlayer.radius + _puck.radius;

    if (collided_first)
    {
#ifdef _DEBUG
        std::cout << "collided first\n";
#endif
        handleCollision(firstPlayer);
    }
    if (collided_second)
    {
#ifdef _DEBUG
        std::cout << "collided second\n";
#endif
        handleCollision(secondPlayer);
    }
}

void Logic::handleCollision(Player& p)
{
    auto x = _puck.pos - _puck.oldPos;
    _puck.pos -= x;

    if (p.speed == math::Vector2D::Zero)
    {
        auto d_vec_norm = (p.pos - _puck.pos).normalize();
        auto n_vec_norm = math::normalize(math::normal(d_vec_norm));
        auto I = math::Vector2D(n_vec_norm.x(), d_vec_norm.x()).normalize();
        auto J = math::Vector2D(n_vec_norm.y(), d_vec_norm.y()).normalize();

        auto u_prime = math::Vector2D(math::proj(_puck.speed, n_vec_norm),
                                      -math::proj(_puck.speed, d_vec_norm));
        auto new_speed_norm = math::Vector2D(math::proj(u_prime, I),
                                             math::proj(u_prime, J)).normalize();

        _puck.speed = _puck.speed.length() * new_speed_norm;
    }
    else
    {
        auto speedLimit = 10.0;
        _puck.speed = std::min(p.speed, speedLimit * math::normalize(p.speed));
    }
    Server::getInstance().setCollision(_puck.pos.x(), _puck.speed.length()); //at, volume
}

void Logic::handleWallCollisionsAndGoal()
{
    int lr_border = 40;
    int tb_border = 40;

    auto x_max = defaultScreenWidth - lr_border - _puck.radius / 2;
    auto x_min = _puck.radius / 2 + lr_border;
    auto y_max = defaultScreenHeight - tb_border - _puck.radius/ 2;
    auto y_min = _puck.radius / 2 + tb_border;


    bool goal = checkGoal();

    if (!goal)
    {
        if (_puck.pos.x() > x_max) {
            _puck.pos.x(x_max - (_puck.pos.x() - x_max));
            _puck.speed.x(-_puck.speed.x());
        }
        if (_puck.pos.x() < x_min) {
            _puck.pos.x(x_min + x_min - _puck.pos.x());
            _puck.speed.x(-_puck.speed.x());
        }
        if (_puck.pos.y() > y_max) {
            _puck.pos.y(y_max - (_puck.pos.y() - y_max));
            _puck.speed.y(-_puck.speed.y());
        }
        if (_puck.pos.y() < y_min) {
            _puck.pos.y(y_min + y_min - _puck.pos.y());
            _puck.speed.y(-_puck.speed.y());
        }
    }
}

bool Logic::checkGoal()
{
    int gap_width = 200;
    if (_puck.pos.y() >= gap_width + _puck.radius
            && _puck.pos.y() <= (defaultScreenHeight + gap_width) / 2 - _puck.radius)  // if in goal section
    {
        auto x_min = 0;
        auto x_max = defaultScreenWidth;
        if (_puck.pos.x() > x_max) {
            handleGoal(0);
        }
        if (_puck.pos.x() < x_min) {
            handleGoal(1);
        }
        return true;
    }
    return false;
}

void Logic::handleGoal(int goaler)
{
    float circleRadius = 100.0f;
    float side = goaler;
    if (goaler == 0)
    {
        std::cout << "Left player goaled\n";
        side = +1.0f;
    }
    else
    {
        std::cout << "Right player goaled\n";
        side = -1.0f;
    }
    _players[goaler].points += 1;
    setInitialCoords();
    _puck.pos.x(_puck.pos.x() + side * circleRadius);
    Server::getInstance().setGoal(goaler, player(goaler).points);

    if(player(goaler).points >= _scoresToWin)
    {
        std::cout << "Logic: we have a winner!" << std::endl;

        _winnerId = goaler;
        Server::getInstance().setWinner(_winnerId);
    }
}

bool Logic::shouldStop() const
{
    return _shouldStop;
}

Logic::StopReason Logic::reason() const
{
    return _reason;
}

int Logic::getWinnerId() const
{
    return _winnerId;
}

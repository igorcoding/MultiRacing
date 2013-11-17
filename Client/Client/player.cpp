#include "player.h"

namespace NeonHockey
{
    Player::Player()
        : _name(),
          _points(-1),
          _paddle(nullptr)
    {

    }

    Player::Player(const std::string& name, BoardSide::BoardSide boardSide, int points, Paddle_ptr paddle)
        : _name(name),
          _side(boardSide),
          _points(points),
          _paddle(std::move(paddle))
    {
    }

    const std::string &Player::getName() const
    {
        return _name;
    }

    BoardSide::BoardSide Player::getSide() const
    {
        return _side;
    }

    int Player::getPoints() const
    {
        return _points;
    }

    Paddle* Player::paddle() const
    {
        return _paddle.get();
    }
}

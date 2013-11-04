#include "player.h"

namespace NeonHockey
{
    Player::Player(const std::string& name, BoardSide::BoardSide boardSide, int points, Paddle_ptr paddle)
        : _name(name),
          _side(boardSide),
          _points(points),
          _paddle(std::move(paddle))
    {
    }

    Paddle* Player::paddle() const
    {
        return _paddle.get();
    }
}

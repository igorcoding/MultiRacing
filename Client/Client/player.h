#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>

#include "paddle.h"

namespace NeonHockey
{
    namespace BoardSide
    {
        enum BoardSide
        {
            LEFT,
            RIGHT
        };
    }

    class Player
    {
        using Paddle_ptr = std::unique_ptr<Paddle>;
    public:
        Player();
        Player(const std::string &name, BoardSide::BoardSide boardSide, int points, Paddle_ptr paddle);

        const std::string& getName() const;
        BoardSide::BoardSide getSide() const;
        int getPoints() const;
        Paddle* paddle() const;


    private:
        std::string _name;
        BoardSide::BoardSide _side;
        int _points;
        Paddle_ptr _paddle;
    };

}

#endif // PLAYER_H

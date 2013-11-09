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
            TOP,
            BOTTOM
        };
    }

    class Player
    {
        using Paddle_ptr = std::unique_ptr<Paddle>;
    public:
        Player(const std::string &name, BoardSide::BoardSide boardSide, int points, Paddle_ptr paddle);

        Paddle* paddle() const;


    private:
        std::string _name;
        BoardSide::BoardSide _side;
        int _points;
        Paddle_ptr _paddle;
    };

}

#endif // PLAYER_H

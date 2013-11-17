#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"

struct Player : public GameObject
{
    Player(int side, int x = 0, int y = 0);

    int points;
    int side;
};

#endif // PLAYER_H

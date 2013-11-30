#include "player.h"

Player::Player(int playerSide, int x, int y)
    : GameObject(x, y, 32),
      points(0),
      side(playerSide)
{
}

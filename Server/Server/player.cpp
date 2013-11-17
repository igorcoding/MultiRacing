#include "player.h"

Player::Player(int playerSide, int x, int y)
    : GameObject(x, y),
      points(0),
      side(playerSide)
{
}

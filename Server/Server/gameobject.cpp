#include "gameobject.h"

GameObject::GameObject(int _x, int _y, int _radius)
    : x(_x),
      y(_y),
      dx(0),
      dy(0),
      vx(0),
      vy(0),
      radius(_radius)
{

}

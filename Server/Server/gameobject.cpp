#include "gameobject.h"

GameObject::GameObject(float _x, float _y, int _radius)
    : oldPos(_x, _y),
      pos(_x, _y),
      speed(),
      accel(),
      radius(_radius)
{

}

void GameObject::setOldPos(float x, float y)
{
    oldPos.x(x);
    oldPos.y(y);
}

void GameObject::setPos(float x, float y)
{
    oldPos = pos;

    pos.x(x);
    pos.y(y);

    this->x = x;
    this->y = y;
}

void GameObject::setSpeed(float x, float y)
{
    speed.x(x);
    speed.y(y);

    //const float friction = 2.0f;
    //setAccel(-x / friction, -y / friction);
}

void GameObject::setAccel(float x, float y)
{
    accel.x(x);
    accel.y(y);
}




float GameObject::dx()
{
    return pos.x() - oldPos.x();
}

float GameObject::dy()
{
    return pos.y() - oldPos.y();
}
















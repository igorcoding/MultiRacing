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
    setOldPos(pos);

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

void GameObject::setOldPos(const math::Vector2D& vec)
{
    setOldPos(vec.x(), vec.y());
}

void GameObject::setPos(const math::Vector2D& vec)
{
    setPos(vec.x(), vec.y());
}

void GameObject::setSpeed(const math::Vector2D& vec)
{
    setSpeed(vec.x(), vec.y());
}

void GameObject::setAccel(const math::Vector2D& vec)
{
    setAccel(vec.x(), vec.y());
}


math::Vector2D GameObject::getOldPos()
{
    return oldPos;
}

math::Vector2D GameObject::getPos()
{
    return pos;
}

math::Vector2D GameObject::getSpeed()
{
    return speed;
}

math::Vector2D GameObject::getAccel()
{
    return accel;
}




float GameObject::dx()
{
    return getPos().x() - getOldPos().x();
}

float GameObject::dy()
{
    return getPos().y() - getOldPos().y();
}
















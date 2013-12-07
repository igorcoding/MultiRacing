#include "gameobject.h"

GameObject::GameObject(float _x, float _y, int _radius)
    : oldPos(2),
      pos(2),
      speed(2),
      accel(2),
      radius(_radius)
{
    oldPos[0] = pos[0] = _x;
    oldPos[1] = pos[1] = _y;
    speed[0] = speed[1] = 0;
    accel[0] = accel[1] = 0;
}

void GameObject::setOldPos(float x, float y)
{
    oldPos[0] = x;
    oldPos[1] = y;
}

void GameObject::setPos(float x, float y)
{
    pos[0] = this->x = x;
    pos[1] = this->y = y;
}

void GameObject::setSpeed(float x, float y)
{
    speed[0] = this->vx = x;
    speed[1] = this->vy = y;

    const float friction = 2.0f;
    setAccel(-x / friction, -y / friction);
}

void GameObject::setAccel(float x, float y)
{
    accel[0] = this->ax = x;
    accel[1] = this->ay = y;
}

void GameObject::setOldPos(Coords coords)
{
    setOldPos(coords.x, coords.y);
}

void GameObject::setPos(Coords coords)
{
    setPos(coords.x, coords.y);
}

void GameObject::setSpeed(Coords coords)
{
    setSpeed(coords.x, coords.y);
}


void GameObject::setAccel(Coords coords)
{
    setAccel(coords.x, coords.y);
}

Coords GameObject::getOldPos() const
{
    return Coords(oldPos[0], oldPos[1]);
}

Coords GameObject::getPos() const
{
    return Coords(pos[0], pos[1]);
}

Coords GameObject::getSpeed() const
{
    return Coords(speed[0], speed[1]);
}

Coords GameObject::getAccel() const
{
    return Coords(accel[0], accel[1]);
}

boost::numeric::ublas::vector<float> GameObject::getOldPosVector() const
{
    return oldPos;
}

boost::numeric::ublas::vector<float> GameObject::getPosVector() const
{
    return pos;
}

boost::numeric::ublas::vector<float> GameObject::getSpeedVector() const
{
    return speed;
}

boost::numeric::ublas::vector<float> GameObject::getAccelVector() const
{
    return accel;
}

float GameObject::dx() const
{
    return getPos().x - getOldPos().x;
}

float GameObject::dy() const
{
    return getPos().y - getOldPos().y;
}
















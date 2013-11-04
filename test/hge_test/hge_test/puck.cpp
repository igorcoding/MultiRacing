#include "puck.h"

Puck::Puck()
    : _hitter(nullptr),
      _x(0.0f),
      _y(0.0f),
      _dx(0.0f),
      _dy(0.0f),
      _speed(0.0f),
      _accel(0.0f)
{
}

float Puck::x() const
{
    return _x;
}

float Puck::x(float new_x)
{
    _x = new_x;
    return _x;
}


float Puck::y() const
{
    return _y;
}

float Puck::y(float new_y)
{
    _y = new_y;
    return _y;
}


float Puck::dx() const
{
    return _dx;
}

float Puck::dx(float new_dx)
{
    _dx = new_dx;
    return _dx;
}


float Puck::dy() const
{
    return _dy;
}

float Puck::dy(float new_dy)
{
    _dy = new_dy;
    return _dy;
}


float Puck::speed() const
{
    return _speed;
}

float Puck::speed(float new_speed)
{
    _speed = new_speed;
    return _speed;
}


float Puck::acceleration() const
{
    return _accel;
}

float Puck::acceleration(float new_acceleration)
{
    _accel = new_acceleration;
    return _accel;
}

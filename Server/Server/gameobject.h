#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "vector2d.h"


class GameObject
{
public:
    GameObject(float _x, float _y, int _radius);

    math::Vector2D oldPos;
    math::Vector2D pos;
    math::Vector2D speed;
    int radius;

    void setOldPos(float x, float y);
    void setOldPos();
    void setPos(float x, float y);
    void setSpeed(float x, float y);

    void reset();

    float dx();
    float dy();
};

#endif // GAMEOBJECT_H

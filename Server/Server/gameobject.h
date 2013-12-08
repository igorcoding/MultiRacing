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
    math::Vector2D accel;
    int radius;

    void setOldPos(float x, float y);
    void setPos(float x, float y);
    void setSpeed(float x, float y);
    void setAccel(float x, float y);

    void setOldPos(const math::Vector2D& vec);
    void setPos(const math::Vector2D& vec);
    void setSpeed(const math::Vector2D& vec);
    void setAccel(const math::Vector2D& vec);

    math::Vector2D getOldPos();
    math::Vector2D getPos();
    math::Vector2D getSpeed();
    math::Vector2D getAccel();


    float dx();
    float dy();

    int x;
    int y;
    int vx;
    int vy;
    int ax;
    int ay;
};

#endif // GAMEOBJECT_H

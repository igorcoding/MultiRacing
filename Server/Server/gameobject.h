#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <hgevector.h> //REVIEW: ты шо? hge? на сервере? -_- нет.

class GameObject
{
public:
    GameObject(int _x, int _y, int _radius);

    int x;
    int y;
    hgeVector dir;
    hgeVector speed;
    float dx;
    float dy;
    float vx;
    float vy;
    int radius;
};

#endif // GAMEOBJECT_H

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <boost/numeric/ublas/vector.hpp>

struct Coords
{
    float x;
    float y;
    Coords(float x, float y)
        : x(x),
          y(y)
    { }
};

class GameObject
{
public:
    GameObject(float _x, float _y, int _radius);

    boost::numeric::ublas::vector<float> oldPos;
    boost::numeric::ublas::vector<float> pos;
    boost::numeric::ublas::vector<float> speed;
    boost::numeric::ublas::vector<float> accel;
    int radius;

    void setOldPos(float x, float y);
    void setPos(float x, float y);
    void setSpeed(float x, float y);
    void setAccel(float x, float y);

    void setOldPos(Coords coords);
    void setPos(Coords coords);
    void setSpeed(Coords coords);
    void setAccel(Coords coords);

    Coords getOldPos() const;
    Coords getPos() const;
    Coords getSpeed() const;
    Coords getAccel() const;

    boost::numeric::ublas::vector<float> getOldPosVector() const;
    boost::numeric::ublas::vector<float> getPosVector() const;
    boost::numeric::ublas::vector<float> getSpeedVector() const;
    boost::numeric::ublas::vector<float> getAccelVector() const;


    float dx() const;
    float dy() const;

    int x;
    int y;
    int vx;
    int vy;
    int ax;
    int ay;
};

#endif // GAMEOBJECT_H

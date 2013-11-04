#ifndef PUCK_H
#define PUCK_H

class Player;

class Puck
{
public:
    Puck();
    Puck(const Puck& puck);
    Puck& operator =(const Puck& puck);

    float x() const;
    float x(float new_x);

    float y() const;
    float y(float new_y);

    float dx() const;
    float dx(float new_dx);

    float dy() const;
    float dy(float new_dy);

    float speed() const;
    float speed(float new_speed);

    float acceleration() const;
    float acceleration(float new_acceleration);


private:
    Player* _hitter;
    float _x;
    float _y;
    float _dx;
    float _dy;
    float _speed;
    float _accel;

};

#endif // PUCK_H

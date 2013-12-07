#include "vector2d.h"
#include <cmath>

namespace math
{
    Vector2D Vector2D::Zero = Vector2D();

    Vector2D::Vector2D(int x, int y)
        : _x(x),
          _y(y)
    {
    }

    Vector2D& Vector2D::operator+=(const Vector2D& other)
    {
        this->_x += other._x;
        this->_y += other._y;
        return *this;
    }

    Vector2D& Vector2D::operator-=(const Vector2D& other)
    {
        this->_x -= other._x;
        this->_y -= other._y;
        return *this;
    }

    float Vector2D::length() const
    {
        return std::sqrt(_x * _x + _y * _y);
    }

    void Vector2D::normalize()
    {
        float l = length();
        _x /= l;
        _y /= l;
    }

    float Vector2D::dot(const Vector2D& other) const
    {
        return _x * other._x + _y * other._y;
    }

    float Vector2D::x() const
    {
        return _x;
    }

    void Vector2D::x(float x)
    {
        _x = x;
    }

    float Vector2D::y() const
    {
        return _y;
    }

    void Vector2D::y(float y)
    {
        _y = y;
    }






    Vector2D operator+ (const Vector2D& lhs, const Vector2D& rhs)
    {
        Vector2D res(lhs);
        res += rhs;
        return res;
    }

    Vector2D operator- (const Vector2D& lhs, const Vector2D& rhs)
    {
        Vector2D res(lhs);
        res -= rhs;
        return res;
    }

    float dot(const Vector2D& lhs, const Vector2D& rhs)
    {
        return lhs.dot(rhs);
    }

    Vector2D normalize(const Vector2D& vec)
    {
        Vector2D res(vec);
        if (res != Vector2D::Zero)
            res.normalize();
        return res;
    }

    Vector2D operator* (float lhs, const Vector2D& rhs)
    {
        Vector2D res(rhs);
        res.x(lhs * res.x());
        res.y(lhs * res.y());
        return res;
    }

    Vector2D operator* (const Vector2D& lhs, float rhs)
    {
        Vector2D res(lhs);
        res.x(rhs * res.x());
        res.y(rhs * res.y());
        return res;
    }

    bool operator== (const Vector2D& lhs, const Vector2D& rhs)
    {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    bool operator!= (const Vector2D& lhs, const Vector2D& rhs)
    {
        return !(lhs == rhs);
    }


}

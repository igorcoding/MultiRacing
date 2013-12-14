#include "vector2d.h"
#include <cmath>

namespace math
{
    Vector2D Vector2D::Zero = Vector2D();

    Vector2D::Vector2D(float x, float y)
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

    Vector2D& Vector2D::normalize()
    {
        float l = length();
        _x /= l;
        _y /= l;
        return *this;
    }

    float Vector2D::dot(const Vector2D& other) const
    {
        return _x * other._x + _y * other._y;
    }

    void Vector2D::reset()
    {
        _x = 0.0f;
        _y = 0.0f;
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

    Vector2D operator-(const Vector2D& lhs)
    {
        Vector2D res(lhs);
        res.x(-res.x());
        res.y(-res.y());
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
        return Vector2D(lhs * rhs.x(), lhs * rhs.y());
    }

    Vector2D operator* (const Vector2D& lhs, float rhs)
    {
        return rhs * lhs;
    }

    bool operator== (const Vector2D& lhs, const Vector2D& rhs)
    {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    bool operator!= (const Vector2D& lhs, const Vector2D& rhs)
    {
        return !(lhs == rhs);
    }

    float distance(const Vector2D& lhs, const Vector2D& rhs)
    {
        auto dx = lhs.x() - rhs.x();
        auto dy = lhs.y() - rhs.y();
        return std::sqrt(dx * dx + dy * dy);
    }

    float cos_angle(const Vector2D& v1, const Vector2D& v2)
    {
        return math::dot(math::normalize(v1), math::normalize(v2));
    }

    float angle(const Vector2D& v1, const Vector2D& v2)
    {
        return std::acos(cos_angle(v1, v2));
    }

    Vector2D normal(const Vector2D& vec)
    {
        return Vector2D(vec.y(), -vec.x());
    }

    float proj(const Vector2D& vec, const Vector2D& base)
    {
        return dot(vec, base) / base.length();
    }

    Vector2D operator/ (const Vector2D& lhs, float rhs)
    {
        return Vector2D(lhs.x() / rhs, lhs.y() / rhs);
    }



}


const math::Vector2D& std::max(const math::Vector2D& v1, const math::Vector2D& v2)
{
    if (v1.length() > v2.length())
        return v1;
    return v2;
}

const math::Vector2D& std::min(const math::Vector2D& v1, const math::Vector2D& v2)
{
    if (v1.length() < v2.length())
        return v1;
    return v2;
}

#ifndef VECTOR2D_H
#define VECTOR2D_H

namespace math
{
    class Vector2D
    {
    public:
        Vector2D(float x = 0.0f, float y = 0.0f);

        float x() const;
        void x(float x);
        float y() const;
        void y(float y);

        float length() const;
        Vector2D& normalize();
        float dot(const Vector2D& other) const;
        void reset();

        Vector2D& operator+= (const Vector2D& other);
        Vector2D& operator-= (const Vector2D& other);

        static Vector2D Zero;
    private:
        float _x;
        float _y;
    };

    float dot(const Vector2D& lhs, const Vector2D& rhs);
    Vector2D normalize(const Vector2D& vec);
    Vector2D normal(const Vector2D& vec);
    float distance(const Vector2D& lhs, const Vector2D& rhs);
    float cos_angle(const Vector2D& v1, const Vector2D& v2);
    float angle(const Vector2D& v1, const Vector2D& v2);
    float proj(const Vector2D& vec, const Vector2D& base);

    Vector2D operator+ (const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator- (const Vector2D& lhs);
    Vector2D operator- (const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator* (float lhs, const Vector2D& rhs);
    Vector2D operator* (const Vector2D& lhs, float rhs);
    Vector2D operator/ (const Vector2D& lhs, float rhs);
    bool operator== (const Vector2D& lhs, const Vector2D& rhs);
    bool operator!= (const Vector2D& lhs, const Vector2D& rhs);

}

namespace std
{
    const math::Vector2D& max(const math::Vector2D& v1, const math::Vector2D& v2);
    const math::Vector2D& min(const math::Vector2D& v1, const math::Vector2D& v2);
}

#endif // VECTOR2D_H

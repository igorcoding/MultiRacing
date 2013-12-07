#ifndef VECTOR2D_H
#define VECTOR2D_H

namespace math
{
    class Vector2D
    {
    public:
        Vector2D(int x = 0.0f, int y = 0.0f);

        float x() const;
        void x(float x);
        float y() const;
        void y(float y);

        float length() const;
        void normalize();
        float dot(const Vector2D& other) const;

        Vector2D& operator+= (const Vector2D& other);
        Vector2D& operator-= (const Vector2D& other);

        static Vector2D Zero;
    private:
        float _x;
        float _y;
    };

    float dot(const Vector2D& lhs, const Vector2D& rhs);
    Vector2D normalize(const Vector2D& vec);

    Vector2D operator+ (const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator- (const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator* (float lhs, const Vector2D& rhs);
    Vector2D operator* (const Vector2D& lhs, float rhs);
    bool operator== (const Vector2D& lhs, const Vector2D& rhs);
    bool operator!= (const Vector2D& lhs, const Vector2D& rhs);

}

#endif // VECTOR2D_H

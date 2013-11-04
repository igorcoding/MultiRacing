#ifndef GFXOBJECT_H
#define GFXOBJECT_H

namespace NeonHockey
{
    struct GfxObject
    {
    public:
        float x;
        float y;
        float dx;
        float dy;
        float speed;
        float accel;

        float width() const;
        float height() const;

    protected:
        GfxObject(float width, float height);

    private:
        float _width;
        float _height;
    };

}

#endif // GFXOBJECT_H

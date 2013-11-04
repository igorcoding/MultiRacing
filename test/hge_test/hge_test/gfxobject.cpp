#include "gfxobject.h"

namespace NeonHockey
{
    GfxObject::GfxObject(float width, float height)
        : x(0.0f),
          y(0.0f),
          dx(0.0f),
          dy(0.0f),
          speed(0.0f),
          accel(0.0f),
          _width(width),
          _height(height)
    { }

    float GfxObject::width() const
    {
        return _width;
    }

    float GfxObject::height() const
    {
        return _height;
    }
}

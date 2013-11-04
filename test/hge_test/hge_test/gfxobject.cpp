#include "gfxobject.h"

namespace NeonHockey
{
GfxObject::GfxObject(const TextureInfo& textureInfo)
        : x(0.0f),
          y(0.0f),
          dx(0.0f),
          dy(0.0f),
          speed(0.0f),
          accel(0.0f),
          _textureInfo(textureInfo)
    { }




}

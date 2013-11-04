#include "gfxobject.h"

namespace NeonHockey
{
    GfxObject::GfxObject(const SpriteInfo& textureInfo)
        : x(0.0f),
          y(0.0f),
          dx(0.0f),
          dy(0.0f),
          speed(0.0f),
          accel(0.0f),
          _spriteInfo(textureInfo)
    { }

    const SpriteInfo &GfxObject::spriteInfo() const
    {
        return _spriteInfo;
    }



}

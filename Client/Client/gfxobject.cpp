#include "gfxobject.h"

namespace NeonHockey
{
    GfxObject::GfxObject(const std::shared_ptr<hgeSprite> &sprite, float w, float h)
        : x(0.0f),
          y(0.0f),
          width(w),
          height(h),
          _sprite(sprite)
    { }

    const std::shared_ptr<hgeSprite> GfxObject::sprite() const
    {
        return _sprite;
    }



}

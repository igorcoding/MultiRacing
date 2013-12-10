#include "gfxobject.h"

namespace NeonHockey
{
    GfxObject::GfxObject(const hgeSprite* sprite)
        : x(0.0f),
          y(0.0f),
          _sprite(sprite)
    { }

    const hgeSprite* GfxObject::sprite() const
    {
        return _sprite;
    }



}

#include "puck.h"

namespace NeonHockey
{
    Puck::Puck(const SpriteInfo &textureInfo)
        : GfxObject(textureInfo),
          _hitter(nullptr)
    {
    }

}



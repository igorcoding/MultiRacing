#include "puck.h"

namespace NeonHockey
{
    Puck::Puck(const TextureInfo &textureInfo)
        : GfxObject(textureInfo),
          _hitter(nullptr)
    {
    }

}



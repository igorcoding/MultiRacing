#include "puck.h"


namespace NeonHockey
{
    Puck::Puck(const std::shared_ptr<hgeSprite>& sprite)
        : GfxObject(sprite, 46, 46)
    {
    }

}



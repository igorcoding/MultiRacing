#include "paddle.h"

namespace NeonHockey
{
    Paddle::Paddle(const hgeSprite* sprite)
        : GfxObject(sprite, 40, 40)
    {
    }
}

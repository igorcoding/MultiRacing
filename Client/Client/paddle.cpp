#include "paddle.h"

namespace NeonHockey
{
    Paddle::Paddle(const std::shared_ptr<hgeSprite> &sprite)
        : GfxObject(sprite, 40, 40)
    {
    }
}

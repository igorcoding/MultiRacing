#ifndef PADDLE_H
#define PADDLE_H

#include "gfxobject.h"

namespace NeonHockey
{
    class Paddle : public GfxObject
    {
    public:
        Paddle(const SpriteInfo& spriteInfo);
    };
}

#endif // PADDLE_H

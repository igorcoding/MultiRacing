#ifndef PADDLE_H
#define PADDLE_H

#include "gfxobject.h"

namespace NeonHockey
{
    class Paddle : public GfxObject
    {
    public:
        Paddle(const std::shared_ptr<hgeSprite>& sprite);
    };
}

#endif // PADDLE_H

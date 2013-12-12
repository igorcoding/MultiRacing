#ifndef PUCK_H
#define PUCK_H

#include "player.h"
#include "gfxobject.h"

namespace NeonHockey
{
    class Puck : public GfxObject
    {
    public:
        Puck(const std::shared_ptr<hgeSprite>& sprite);

    private:
        Puck(const Puck& puck) = delete;
        Puck& operator =(const Puck& puck) = delete;
    };

}

#endif // PUCK_H

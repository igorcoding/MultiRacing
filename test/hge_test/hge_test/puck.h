#ifndef PUCK_H
#define PUCK_H

#include "player.h"
#include "gfxobject.h"

namespace NeonHockey
{
    class Puck : public GfxObject
    {
    public:
        Puck(const SpriteInfo& spriteInfo);

    private:
        Player* _hitter;

        Puck(const Puck& puck);
        Puck& operator =(const Puck& puck);
    };

}

#endif // PUCK_H

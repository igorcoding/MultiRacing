#ifndef GFXOBJECT_H
#define GFXOBJECT_H

#include <hgesprite.h>

namespace NeonHockey
{
    struct GfxObject
    {
    public:
        float x;
        float y;

        const hgeSprite* sprite() const;

    protected:
        GfxObject(const hgeSprite* sprite);

    private:
        const hgeSprite* _sprite;
    };

}

#endif // GFXOBJECT_H

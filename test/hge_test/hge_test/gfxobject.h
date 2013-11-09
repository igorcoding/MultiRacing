#ifndef GFXOBJECT_H
#define GFXOBJECT_H

#include "unix_platform.h"
#include "spriteinfo.h"

namespace NeonHockey
{
    struct GfxObject
    {
    public:
        float x;
        float y;
        float dx;
        float dy;
        float speed;
        float accel;

        const SpriteInfo& spriteInfo() const;

    protected:
        GfxObject(const SpriteInfo& spriteInfo);

    private:
        const SpriteInfo& _spriteInfo;
    };

}

#endif // GFXOBJECT_H

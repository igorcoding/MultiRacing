#ifndef GFXOBJECT_H
#define GFXOBJECT_H

#include "unix_platform.h"
#include "textureinfo.h"

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

    protected:
        GfxObject(const TextureInfo& textureInfo);

    private:
        TextureInfo _textureInfo;
    };

}

#endif // GFXOBJECT_H

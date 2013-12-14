#ifndef GFXOBJECT_H
#define GFXOBJECT_H

#include <hgesprite.h>
#include <memory>

namespace NeonHockey
{
    struct GfxObject
    {
    public:
        float x;
        float y;
        float width;
        float height;
        float initX;
        float initY;

        void setInitPos(float x, float y);
        void resetToInit();

        const std::shared_ptr<hgeSprite> sprite() const;

    protected:
        GfxObject(const std::shared_ptr<hgeSprite>& sprite, float w, float h);

    private:
        const std::shared_ptr<hgeSprite> _sprite;
    };

}

#endif // GFXOBJECT_H

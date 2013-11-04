#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "unix_platform.h"

#include <vector>
#include <map>

#include <hge.h>
#include <hgesprite.h>

namespace NeonHockey
{
    namespace GfxType
    {
        enum GfxObjectType {
            PUCK,
            PADDLE
        };
    }

    class ResourceManager
    {
    public:
        ResourceManager(HGE* hge = nullptr);
        ~ResourceManager();

        void InitHge(HGE *hge);
        void FreeResources();
        void AddTexture(GfxType::GfxObjectType gfxType, const char* filename, DWORD size = 0, bool bMipmap = false);
        void AddSprite(GfxType::GfxObjectType gfxType, float x, float y, float w, float h);

        HTEXTURE GetTexture(GfxType::GfxObjectType gfxType);
        hgeSprite* GetSprite(GfxType::GfxObjectType gfxType);

    private:
        HGE* _hge;
        std::map<GfxType::GfxObjectType, HTEXTURE> _textures;
        std::map<GfxType::GfxObjectType, hgeSprite*> _sprites;
        bool _clear;
    };

}

#endif // RESOURCEMANAGER_H

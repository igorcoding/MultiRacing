#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <hge.h>
#include <hgesprite.h>

#include <map>
#include <string>



namespace NeonHockey
{
    namespace GfxType
    {
        enum GfxObjectType {
            PUCK,
            PADDLE1,
            PADDLE2
        };
    }

    class ResourceManager
    {
    public:
        ResourceManager(HGE* hge = nullptr);
        ~ResourceManager();

        void initHge(HGE *hge);
        void freeResources();
        void freeResource(GfxType::GfxObjectType gfxType);
        void addTexture(GfxType::GfxObjectType gfxType, std::string filename, DWORD size = 0, bool bMipmap = false);
        void addTexture(GfxType::GfxObjectType gfxType, const char* filename, DWORD size = 0, bool bMipmap = false);
        hgeSprite* addSprite(GfxType::GfxObjectType gfxType, float x, float y, float w, float h);

        HTEXTURE getTexture(GfxType::GfxObjectType gfxType);
        hgeSprite* getSprite(GfxType::GfxObjectType gfxType);

    private:
        HGE* _hge;
        std::map<GfxType::GfxObjectType, HTEXTURE> _textures;
        std::map<GfxType::GfxObjectType, hgeSprite*> _sprites;
        bool _clear;
    };

}

#endif // RESOURCEMANAGER_H

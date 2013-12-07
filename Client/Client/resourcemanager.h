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
        enum GfxObjectType { //REVIEW: эти GfxType::GfxObjectType выглядят ужасно
            PUCK,            //если ты не используешь индексацию числами (magic'ми)
            PADDLE_CURRENT,  //то стоит просто перейти на enum class
            PADDLE_ENEMY,
            BACKGROUND
        };
    }

    namespace SoundType
    {
        enum SoundObjectType {
            COLLISION,
            GOAL
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
        void freeResource(SoundType::SoundObjectType sndType);

        void addTexture(GfxType::GfxObjectType gfxType, std::string filename, DWORD size = 0, bool bMipmap = false);
        void addTexture(GfxType::GfxObjectType gfxType, const char* filename, DWORD size = 0, bool bMipmap = false);
        hgeSprite* addSprite(GfxType::GfxObjectType gfxType, float x, float y, float w, float h);
        void addSound(SoundType::SoundObjectType sndType, std::string filename);


        HTEXTURE getTexture(GfxType::GfxObjectType gfxType);
        hgeSprite* getSprite(GfxType::GfxObjectType gfxType);
        HEFFECT getSound(SoundType::SoundObjectType sndType);

    private:
        HGE* _hge;
        std::map<GfxType::GfxObjectType, HTEXTURE> _textures;
        std::map<GfxType::GfxObjectType, hgeSprite*> _sprites;
        std::map<SoundType::SoundObjectType, HEFFECT> _sounds;

        bool _clear;
    };
}

#endif // RESOURCEMANAGER_H

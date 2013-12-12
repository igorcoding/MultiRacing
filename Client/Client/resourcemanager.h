#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>

#include <map>
#include <string>
#include <memory>

#include "spriteinfo.h"

namespace NeonHockey
{
    enum class GfxType
    {
        PUCK,
        PADDLE,
        BACKGROUND,
        INPUTFIELD,
        BUTTON
    };

    enum class SoundType
    {
        COLLISION,
        GOAL
    };

    enum class FontType
    {
        SCORE
    };

    class ResourceManager
    {
    public:
        ResourceManager(HGE* hge = nullptr);
        ~ResourceManager();

        void initHge(HGE *hge);
        void freeResources();
        void freeResource(GfxType gfxType);
        void freeResource(SoundType sndType);
        void freeResource(FontType fntType);

        void addTexture(GfxType gfxType, std::string filename, DWORD size = 0, bool bMipmap = false);
        void addTexture(GfxType gfxType, const char* filename, DWORD size = 0, bool bMipmap = false);
        std::shared_ptr<hgeSprite> addSprite(GfxType gfxType, float x, float y, float w, float h);
        std::shared_ptr<hgeSprite> addSprite(GfxType gfxType, const SpriteInfo& spriteInfo);
        void addSound(SoundType sndType, std::string filename);
        void addFont(FontType fntType, std::string filename, bool mipMap = false);

        HTEXTURE getTexture(GfxType gfxType);
        std::shared_ptr<hgeSprite> getSprite(GfxType gfxType);
        HEFFECT getSound(SoundType sndType);
        std::shared_ptr<hgeFont> getFont(FontType fntType);

    private:
        HGE* _hge;
        std::map<GfxType, HTEXTURE> _textures;
        std::map<GfxType, std::shared_ptr<hgeSprite>> _sprites;
        std::map<SoundType, HEFFECT> _sounds;
        std::map<FontType, std::shared_ptr<hgeFont>> _fonts;

        bool _clear;
    };
}

#endif // RESOURCEMANAGER_H

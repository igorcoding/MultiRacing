#include "resourcemanager.h"
#include "exceptions.h"

namespace NeonHockey
{
    ResourceManager::ResourceManager(HGE* hge)
        : _hge(hge),
          _clear(true)
    { }

    ResourceManager::~ResourceManager()
    {
        if (!_clear)
            freeResources();
    }

    void ResourceManager::initHge(HGE* hge)
    {
        _hge = hge;
    }

    void ResourceManager::freeResources()
    {
        //for (auto it = _sprites.begin(); it != _sprites.end(); ++it)
        //    delete it->second;
        for (auto it = _textures.begin(); it != _textures.end(); ++it)
            _hge->Texture_Free(it->second);

        for(auto &s: _sounds)
            _hge->Effect_Free(s.second);

        //for(auto &f: _fonts)
        //    delete f.second;

        _sprites.clear();
        _textures.clear();
        _sounds.clear();
        _fonts.clear();
        _clear = true;
    }

    void ResourceManager::freeResource(GfxType gfxType)
    {
        auto foundTexture = _textures.find(gfxType);
        if (foundTexture != _textures.end())            // if there is a texture under gfxType
        {
            auto foundSprite = _sprites.find(gfxType);  // search for the sprite
            if (foundSprite != _sprites.end())          // delete sprite
            {
                //delete foundSprite->second;
                _sprites.erase(foundSprite);
            }
            _hge->Texture_Free(foundTexture->second);   // delete texture
            _textures.erase(foundTexture);
        }
    }

    void ResourceManager::freeResource(SoundType sndType)
    {
        auto foundEffect = _sounds.find(sndType);
        if (foundEffect != _sounds.end())
        {
            _hge->Effect_Free(foundEffect->second);
            _sounds.erase(foundEffect);
        }
    }

    void ResourceManager::freeResource(FontType fntType)
    {
        auto font = _fonts.find(fntType);
        if (font != _fonts.end())
        {
            //delete font->second;
            _fonts.erase(font);
        }
    }

    void ResourceManager::addTexture(GfxType gfxType, std::string filename, DWORD size, bool bMipmap)
    {
        addTexture(gfxType, filename.c_str(), size, bMipmap);
    }

    void ResourceManager::addTexture(GfxType gfxType, const char *filename, DWORD size, bool bMipmap)
    {
        HTEXTURE texture = _hge->Texture_Load(filename, size, bMipmap);
        if (!texture)
            throw ResourceException(_hge->System_GetErrorMessage());  // TODO: not sure if we need to clean up all the resources here

        freeResource(gfxType); // delete resource if it is already initialized
        _textures[gfxType] = texture;
        if (_clear)
            _clear = false;
    }

    std::shared_ptr<hgeSprite> ResourceManager::addSprite(GfxType gfxType, float x, float y, float w, float h)
    {
        auto foundKey = _textures.find(gfxType);
        if (foundKey == _textures.end())
            throw ResourceException(_hge->System_GetErrorMessage());  // TODO: not sure if we need to clean up all the resources here

        _sprites[gfxType] = std::make_shared<hgeSprite>(foundKey->second, x, y, w, h);
        if (_clear)
            _clear = false;
        return _sprites[gfxType];
    }

    std::shared_ptr<hgeSprite> ResourceManager::addSprite(GfxType gfxType, const SpriteInfo& spriteInfo)
    {
        return addSprite(gfxType, spriteInfo.xTexturePos(), spriteInfo.yTexturePos(), spriteInfo.width(), spriteInfo.height());
    }

    void ResourceManager::addSound(SoundType sndType, std::string filename)
    {
        HEFFECT effect = _hge->Effect_Load(filename.c_str());
        if (!effect)
            throw ResourceException(_hge->System_GetErrorMessage());

        freeResource(sndType); // delete resource if it is already initialized
        _sounds[sndType] = effect;
        if (_clear)
            _clear = false;
    }

    void ResourceManager::addFont(FontType fntType, std::string filename, bool mipMap)
    {
        auto fnt = std::make_shared<hgeFont>(filename.c_str(), mipMap);
        if (!fnt) //TODO: это никогда не сработает, даже если загрузка не прошла
            throw ResourceException(_hge->System_GetErrorMessage());

        freeResource(fntType); // delete resource if it is already initialized
        _fonts[fntType] = fnt;
        if (_clear)
            _clear = false;
    }

    HTEXTURE ResourceManager::getTexture(GfxType gfxType)
    {
        auto foundKey = _textures.find(gfxType);
        if (foundKey == _textures.end())
            throw ResourceException(_hge->System_GetErrorMessage());
        return foundKey->second;
    }

    std::shared_ptr<hgeSprite> ResourceManager::getSprite(GfxType gfxType)
    {
        auto foundKey = _sprites.find(gfxType);
        if (foundKey == _sprites.end())
            throw ResourceException(_hge->System_GetErrorMessage());
        return foundKey->second;
    }

    HEFFECT ResourceManager::getSound(SoundType sndType)
    {
        auto foundKey = _sounds.find(sndType);
        if (foundKey == _sounds.end())
            throw ResourceException(_hge->System_GetErrorMessage());
        return foundKey->second;
    }

    std::shared_ptr<hgeFont> ResourceManager::getFont(FontType fntType)
    {
        auto foundKey = _fonts.find(fntType);
        if (foundKey == _fonts.end())
            throw ResourceException(_hge->System_GetErrorMessage());
        return foundKey->second;
    }
}

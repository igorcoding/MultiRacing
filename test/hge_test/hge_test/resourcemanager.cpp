#include "resourcemanager.h"

namespace NeonHockey
{
    ResourceManager::ResourceManager(HGE* hge)
        : _hge(hge),
          _clear(true)
    { }

    ResourceManager::~ResourceManager()
    {
        if (!_clear)
            FreeResources();
    }

    void ResourceManager::InitHge(HGE* hge)
    {
        _hge = hge;
    }

    void ResourceManager::FreeResources()
    {
        for (auto it = _sprites.begin(); it != _sprites.end(); ++it)
            delete it->second;
        for (auto it = _textures.begin(); it != _textures.end(); ++it)
            _hge->Texture_Free(it->second);
        _sprites.clear();
        _textures.clear();
        _clear = true;
    }

    void ResourceManager::FreeResource(GfxType::GfxObjectType gfxType)
    {
        auto foundTexture = _textures.find(gfxType);
        if (foundTexture != _textures.end())            // if there is a texture under gfxType
        {
            auto foundSprite = _sprites.find(gfxType);  // search for the sprite
            if (foundSprite != _sprites.end())          // delete sprite
            {
                delete foundSprite->second;
                _sprites.erase(foundSprite);
            }
            _hge->Texture_Free(foundTexture->second);   // delete texture
            _textures.erase(foundTexture);
        }
    }

    void ResourceManager::AddTexture(GfxType::GfxObjectType gfxType, const char *filename, DWORD size, bool bMipmap)
    {
        HTEXTURE texture = _hge->Texture_Load(filename, size, bMipmap);
        if (!texture)
            throw std::exception();  // TODO: not sure if we need to clean up all the resources here

        FreeResource(gfxType); // delete resource if it is already initialized
        _textures[gfxType] = texture;
        if (_clear)
            _clear = false;
    }

    hgeSprite* ResourceManager::AddSprite(GfxType::GfxObjectType gfxType, float x, float y, float w, float h)
    {
        auto foundKey = _textures.find(gfxType);
        if (foundKey == _textures.end())
            throw std::exception();  // TODO: not sure if we need to clean up all the resources here

        _sprites[gfxType] = new hgeSprite(foundKey->second, x, y, w, h);
        if (_clear)
            _clear = false;
        return _sprites[gfxType];
    }

    HTEXTURE ResourceManager::GetTexture(GfxType::GfxObjectType gfxType)
    {
        auto foundKey = _textures.find(gfxType);
        if (foundKey == _textures.end())
            throw std::exception();  // TODO: not sure if we need to clean up all the resources here
        return foundKey->second;
    }

    hgeSprite* ResourceManager::GetSprite(GfxType::GfxObjectType gfxType)
    {
        auto foundKey = _sprites.find(gfxType);
        if (foundKey == _sprites.end())
            throw std::exception();  // TODO: not sure if we need to clean up all the resources here
        return foundKey->second;
    }

}

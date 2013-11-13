#include "spriteinfo.h"

namespace NeonHockey
{
    SpriteInfo::SpriteInfo(float xTexturePos, float yTexturePos, float width, float height, const std::string &texturePath)
        :_xTexturePos(xTexturePos),
         _yTexturePos(yTexturePos),
         _width(width),
         _height(height),
         _texturePath(texturePath)
    { }

    float SpriteInfo::xTexturePos() const
    {
        return _xTexturePos;
    }

    float SpriteInfo::yTexturePos() const
    {
        return _yTexturePos;
    }

    float SpriteInfo::width() const
    {
        return _width;
    }

    float SpriteInfo::height() const
    {
        return _height;
    }

    const std::string& SpriteInfo::texturePath() const
    {
        return _texturePath;
    }


}

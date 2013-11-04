#include "textureinfo.h"

namespace NeonHockey
{
    TextureInfo::TextureInfo(float xTexturePos, float yTexturePos, float width, float height, const std::string &texturePath)
        :_xTexturePos(xTexturePos),
         _yTexturePos(yTexturePos),
         _width(width),
         _height(height),
         _texturePath(texturePath)
    { }

    float TextureInfo::xTexturePos() const
    {
        return _xTexturePos;
    }

    float TextureInfo::yTexturePos() const
    {
        return _yTexturePos;
    }

    float TextureInfo::width() const
    {
        return _width;
    }

    float TextureInfo::height() const
    {
        return _height;
    }

    std::string TextureInfo::texturePath() const
    {
        return _texturePath;
    }


}

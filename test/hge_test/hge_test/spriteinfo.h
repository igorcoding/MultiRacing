#ifndef TEXTUREINFO_H
#define TEXTUREINFO_H

#include <string>

namespace NeonHockey
{
    class SpriteInfo
    {
    public:
        SpriteInfo(float xTexturePos, float yTexturePos, float width, float height, const std::string& texturePath);
        float xTexturePos() const;
        float yTexturePos() const;
        float width() const;
        float height() const;
        const std::string& texturePath() const;

    public:
        float _xTexturePos;
        float _yTexturePos;
        float _width;
        float _height;
        std::string _texturePath;
    };

}

#endif // TEXTUREINFO_H

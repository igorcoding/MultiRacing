#ifndef TEXTUREINFO_H
#define TEXTUREINFO_H

#include <string>

namespace NeonHockey
{
    class TextureInfo
    {
    public:
        TextureInfo(float xTexturePos, float yTexturePos, float width, float height, const std::string& texturePath);
        float xTexturePos() const;
        float yTexturePos() const;
        float width() const;
        float height() const;
        std::string texturePath() const;

    private:
        float _xTexturePos;
        float _yTexturePos;
        float _width;
        float _height;
        std::string _texturePath;
    };

}

#endif // TEXTUREINFO_H

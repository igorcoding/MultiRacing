#include "resourcesloader.h"

namespace NeonHockey
{    
    void ResourcesLoader::loadGraphics(ResourceManager* rm)
    {

    }

    void ResourcesLoader::loadFonts(ResourceManager* rm)
    {
        rm.addFont(FontType::SCORE, "../resources/Digital.fnt");
    }

    void ResourcesLoader::loadSound(ResourceManager* rm)
    {
        std::string filename = "../resources/hit";
#ifdef PLATFORM_UNIX
        filename += ".ogg";
#else
        filename += ".wav";
#endif
        rm.addSound(SoundType::COLLISION, filename);
    }

}

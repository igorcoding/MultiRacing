#ifndef RESOURCESLOADER_H
#define RESOURCESLOADER_H

#include <vector>

#include "resourcemanager.h"

namespace NeonHockey
{
    class ResourcesLoader
    {
    public:
        ResourcesLoader(ResourceManager* rm);
        void loadEverything();
        void loadGraphics();
        void loadFonts();
        void loadSound();

    private:
        std::vector<SpriteInfo> _gfxResources;
        ResourceManager* _rm;

        ResourcesLoader(const ResourcesLoader&) = delete;
        ResourcesLoader& operator =(const ResourcesLoader&) = delete;
    };
}

#endif // RESOURCESLOADER_H


#ifndef RESOURCESLOADER_H
#define RESOURCESLOADER_H

#include "resourcemanager.h"

namespace NeonHockey
{
    struct ResourcesLoader
    {
        static void loadGraphics(ResourceManager* rm);
        static void loadFonts(ResourceManager* rm);
        static void loadSound(ResourceManager* rm);
    };
}

#endif // RESOURCESLOADER_H


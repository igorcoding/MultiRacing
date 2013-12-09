#ifndef ICONTEXT_H
#define ICONTEXT_H

#include <hge.h>

#include "resourcemanager.h"

namespace NeonHockey
{

    class IContext
    {
    public:
        IContext(HGE* hge, ResourceManager* rm)
            : _hge(hge),
              _rm(rm)
        { }
        virtual void frameFunc() = 0;
        virtual void renderFunc() = 0;
    private:
        HGE* _hge;
        ResourceManager* _rm;
    };

}

#endif // ICONTEXT_H

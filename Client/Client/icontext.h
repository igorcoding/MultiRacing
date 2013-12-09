#ifndef ICONTEXT_H
#define ICONTEXT_H

#include <hge.h>

#include "resourcemanager.h"

namespace NeonHockey
{
    enum class Context
    {
        NoContext,
        MenuContext,
        ConnectionContext,
        InGameContext,
        GameFinishedContext,
        GameErrorContext,
        ConnectContext
    };

    struct IContextData
    {
        int screenWidth;
        int screenHeight;
        virtual ~IContextData() {}
    };

    class IContext
    {
    public:
        IContext(HGE* hge, ResourceManager* rm, IContextData* data)
            : _hge(hge),
              _rm(rm),
              _data(data)
        { }

        virtual ~IContext()
        {
            delete _data;
            _data = nullptr;
        }

        virtual void start() = 0;
        virtual void show() = 0;
        virtual Context frameFunc() = 0;
        virtual void renderFunc() = 0;
    protected:
        HGE* _hge;
        ResourceManager* _rm;
        IContextData* _data;
    };

}

#endif // ICONTEXT_H

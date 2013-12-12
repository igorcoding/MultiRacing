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
        ConnectContext,
        InGameContext,
        GameFinishedContext,
        GameErrorContext
    };

    struct IContextData
    {
        IContextData(int width, int height)
            : screenWidth(width),
              screenHeight(height)
        { }

        virtual ~IContextData() {}

        int screenWidth;
        int screenHeight;

    };

    struct IContextReturnData
    {
        Context c;
        std::shared_ptr<IContextData> data;

        IContextReturnData(Context c, std::shared_ptr<IContextData> data)
            : c(c),
              data(data)
        { }
    };


    class IContext
    {
    public:
        IContext(HGE* hge, std::shared_ptr<ResourceManager> rm, std::shared_ptr<IContextData> data)
            : _hge(hge),
              _rm(rm),
              _data(data)
        { }

        virtual ~IContext()
        {
            //delete _data;
            //_data = nullptr;
        }

        virtual void show() = 0;
        virtual IContextReturnData frameFunc() = 0;
        virtual void renderFunc() = 0;
        void changeData(std::shared_ptr<IContextData> data)
        {
            //if (_data != data)
            //    delete _data;
            _data = data;

        }

    protected:
        HGE* _hge;
        std::shared_ptr<ResourceManager> _rm;
        std::shared_ptr<IContextData> _data;
    };

}

#endif // ICONTEXT_H

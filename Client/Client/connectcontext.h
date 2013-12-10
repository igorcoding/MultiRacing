#ifndef CONNECTCONTEXT_H
#define CONNECTCONTEXT_H
#include <icontext.h>
#include "inputfield.h"

namespace NeonHockey
{
    struct ConnectContextData : public IContextData
    {
        ConnectContextData(int width, int height)
            : IContextData(width, height)
        { }
    };

    class ConnectContext : public IContext
    {
    public:
        ConnectContext(HGE* hge, ResourceManager* rm, ConnectContextData* data);

        // IContext interface
    public:
        void show();
        IContextReturnData frameFunc();
        void renderFunc();

    private:
        InputField *input;
    };
}

#endif // CONNECTCONTEXT_H

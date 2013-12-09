#ifndef CONNECTCONTEXT_H
#define CONNECTCONTEXT_H
#include <icontext.h>
#include "inputfield.h"

namespace NeonHockey
{
    class ConnectContext : public IContext
    {
    public:
        ConnectContext(HGE* hge, ResourceManager* rm, IContextData* data);

        // IContext interface
    public:
        void show();
        Context frameFunc();
        void renderFunc();

    private:
        InputField *input;
    };
}

#endif // CONNECTCONTEXT_H

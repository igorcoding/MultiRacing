#ifndef INGAMECONTEXT_H
#define INGAMECONTEXT_H

#include "icontext.h"

namespace NeonHockey
{

    class InGameContext : public IContext
    {
    public:
        InGameContext(HGE* hge, ResourceManager* rm);

        virtual void frameFunc();
        virtual void renderFunc();

    private:

    };

}

#endif // INGAMECONTEXT_H

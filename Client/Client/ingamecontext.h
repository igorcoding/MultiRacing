#ifndef INGAMECONTEXT_H
#define INGAMECONTEXT_H

#include "icontext.h"

class InGameContext : public IContext
{
public:
    InGameContext();

    virtual void load(HGE *hge);
    virtual void loadResources();
    virtual void frameFunc();
    virtual void renderFunc();

private:
    HGE* _hge;
};

#endif // INGAMECONTEXT_H

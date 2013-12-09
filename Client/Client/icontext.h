#ifndef ICONTEXT_H
#define ICONTEXT_H

#include <hge.h>

class IContext
{
public:
    IContext(HGE* hge)
        : _hge(hge)
    { }
    virtual void frameFunc() = 0;
    virtual void renderFunc() = 0;
private:
    HGE* _hge;
};

#endif // ICONTEXT_H

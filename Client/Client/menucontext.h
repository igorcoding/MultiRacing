#ifndef MENUCONTEXT_H
#define MENUCONTEXT_H
#include <hgegui.h>
#include <hgeguictrls.h>
#include <vector>
#include "icontext.h"

namespace NeonHockey
{

    struct MenuContextData : public IContextData
    {

    };

    class MenuContext : public IContext
    {
    public:
        MenuContext(HGE* hge, ResourceManager* rm, MenuContextData* data);
        ~MenuContext();

        // IContext interface
    public:
        Context frameFunc();
        void renderFunc();

        void start() {}
        void show() {}
        void itemSet(float dt);
        void enter();
    private:
        hgeGUI *menu;

        int id;
        int lastId;

        std::vector<std::pair<std::string, std::string>> items;
    };
}

#endif // MENUCONTEXT_H

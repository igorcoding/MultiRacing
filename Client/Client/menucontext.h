#ifndef MENUCONTEXT_H
#define MENUCONTEXT_H
#include <hgegui.h>
#include <hgeguictrls.h>
#include <vector>
#include "icontext.h"

namespace NeonHockey
{
    class MenuContext : IContext
    {
    public:
        MenuContext();

        // IContext interface
    public:
        void frameFunc();
        void renderFunc();

        void show();
        void itemSet(float dt);
    private:
        hgeGUI *menu;

        int id;
        int lastid;

        std::vector<std::pair<std::string, std::string>> items;
    };
}

#endif // MENUCONTEXT_H

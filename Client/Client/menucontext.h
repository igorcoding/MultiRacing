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
        MenuContextData(int width, int height)
            : IContextData(width, height)
        { }
    };

    struct ItemType { enum {Null, Connect, About, Exit}; };

    class MenuContext : public IContext
    {
    public:
        MenuContext(HGE* hge, ResourceManager* rm, MenuContextData* data);
        ~MenuContext();

        // IContext interface
    public:
        IContextReturnData frameFunc();
        void renderFunc();

        void show();
        void enter();
    private:
        hgeGUI *menu;

        int id;
        int lastId;

        std::vector<std::pair<std::string, std::string>> items;
    };
}

#endif // MENUCONTEXT_H

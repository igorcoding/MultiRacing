#include "menucontext.h"
#include "menuitem.h"


namespace NeonHockey
{
    MenuContext::MenuContext(HGE* hge, ResourceManager* rm, MenuContextData* data)
        : IContext(hge, rm, data)
    {
        menu = new hgeGUI();

        DWORD shadowColor = ARGB(200, 255, 0, 0);
        DWORD itemColor = ARGB(255, 0, 255, 0);

        std::vector<std::pair<std::string, std::string>> items;
        items.push_back(std::make_pair("Connect!", "Connect to dedicated server"));
        items.push_back(std::make_pair("About", "Information about NeonHockey"));
        items.push_back(std::make_pair("Exit", "Exit game"));


        for(int i = 0; i < items.size(); ++i)
            menu->AddCtrl(
                new hgeGUIMenuItem(i + 1,
                   _rm->getFont(FontType::SCORE),
                   _rm->getSound(SoundType::COLLISION),
                   400, 200+40*i, float(i)/10.0,
                   (char*)items[i].first.c_str(),
                   itemColor, shadowColor));

        menu->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
    }


    Context MenuContext::frameFunc()
    {
        float dt = _hge->Timer_GetDelta();
        itemSet(dt);
    }

    void MenuContext::renderFunc()
    {
        auto statusFont = _rm->getFont(FontType::SCORE);

        menu->Render();
        int id = menu->GetFocus();
        if(id) //TODO: too many magics
            statusFont->Render(10, 800-32, 0, items[id-1].second.c_str());

    }

    MenuContext::~MenuContext()
    {
        delete menu;
    }

    void MenuContext::itemSet(float dt)
    {
        id = menu->Update(dt);

        if(id == -1)
        {
            switch(lastid)
            {
            case 1:
            case 2:
            case 3:
                lastid = 0;
                menu->Enter();
                menu->SetFocus(1);
                break;
            }
        }
        else if(id)
        {
            lastid = id;
            menu->Leave();
        }
    }

    void MenuContext::enter()
    {
        menu->Enter();
        menu->SetFocus(1);
    }
}

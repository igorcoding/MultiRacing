#include "connectcontext.h"
#include "ingamecontext.h"

namespace NeonHockey
{
    ConnectContext::ConnectContext(HGE* hge, ResourceManager* rm, ConnectContextData *data)
        : IContext(hge, rm, data)
    {
        gui = new hgeGUI();

        ipField = new InputField(_rm, ControlId::Ip, 100, 100, "127.0.0.1");
        nameField = new InputField(_rm, ControlId::Name, 100, 100 + 48, "Player 0");

        HTEXTURE buttonTexture = _rm->getTexture(GfxType::BUTTON);
        connectButton = new hgeGUIButton(ControlId::Connect, 86, 190, 100, 28, buttonTexture, 0, 0);

        gui->AddCtrl(ipField);
        gui->AddCtrl(nameField);
        gui->AddCtrl(connectButton);
    }

    ConnectContext::~ConnectContext()
    {
        delete connectButton;
        connectButton = nullptr;

        delete nameField;
        nameField = nullptr;

        delete ipField;
        ipField = nullptr;
    }

    void ConnectContext::show()
    {
        gui->Enter();
    }

    IContextReturnData ConnectContext::frameFunc()
    {
        float dt = _hge->Timer_GetDelta();
        if(gui->Update(dt) == ControlId::Connect)
            return IContextReturnData(Context::InGameContext,
                new InGameContextData(_data->screenWidth, _data->screenHeight, 0));

        return IContextReturnData(Context::ConnectContext, _data);
    }

    void ConnectContext::renderFunc()
    {
        hgeFont *font = _rm->getFont(FontType::SCORE);
        font->Render(74, 100, HGETEXT_RIGHT, "IP: ");

        font->Render(74, 148, HGETEXT_RIGHT, "Name: ");

        gui->Render();

        font->Render(100, 100+48*2, HGETEXT_LEFT, "Connect");

        font->Render(10, _data->screenHeight - font->GetHeight()*1.5,
                     HGETEXT_LEFT, "Input server ip, your nickname and click Connect");
    }
}


#include "connectcontext.h"

namespace NeonHockey
{
    ConnectContext::ConnectContext(HGE* hge, ResourceManager* rm, ConnectContextData *data)
        : IContext(hge, rm, data)
    {
        input = new InputField(_rm, 1, 100, 100, "127.0.0.1");
    }

    void ConnectContext::show()
    {
        input->Enter();
        input->MouseLButton(true);
        input->Focus(true);
    }

    IContextReturnData ConnectContext::frameFunc()
    {
        float dt = _hge->Timer_GetDelta();
        input->Update(dt);

        hgeInputEvent event;
        _hge->Input_GetEvent(&event);

        //input->MouseLButton(_hge->Input_GetKeyState(HGEK_LBUTTON));

        if(event.type == INPUT_KEYDOWN)
        {
            input->KeyClick(event.key, event.chr);

            if(event.key == HGEK_ENTER)
            {
                //try to start connection

                return IContextReturnData(Context::InGameContext, _data);
            }
        }

        return IContextReturnData(Context::ConnectContext, _data);
    }

    void ConnectContext::renderFunc()
    {
        hgeFont *font = _rm->getFont(FontType::SCORE);
        font->Render(60, 100, HGETEXT_LEFT, "IP: ");

        input->Render();

        font->Render(10, _data->screenHeight - font->GetHeight()*1.5,
                     HGETEXT_LEFT, "Input server ip and press Enter");
    }
}


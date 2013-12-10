#include "connectcontext.h"

namespace NeonHockey
{
    ConnectContext::ConnectContext(HGE* hge, ResourceManager* rm, ConnectContextData *data)
        : IContext(hge, rm, data)
    {
        hgeFont *font = _rm->getFont(FontType::SCORE);

        input = new InputField(font, 1, 100, 100, 16, "127.0.0.1");
    }

    void ConnectContext::show()
    {
        input->Enter();
    }

    IContextReturnData ConnectContext::frameFunc()
    {
        float dt = _hge->Timer_GetDelta();
        input->Update(dt);

        return IContextReturnData(Context::ConnectContext, _data);
    }

    void ConnectContext::renderFunc()
    {
        input->Render();
    }
}


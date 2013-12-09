#include "connectcontext.h"

namespace NeonHockey
{
    ConnectContext::ConnectContext(HGE* hge, ResourceManager* rm, IContextData* data)
        : IContext(hge, rm, data)
    {
        hgeFont *font = _rm->getFont(FontType::SCORE);

        input = new InputField(font, 1, 100, 100, 16, "127.0.0.1");
    }

    void ConnectContext::show()
    {
        input->Enter();
    }

    Context ConnectContext::frameFunc()
    {
        float dt = _hge->Timer_GetDelta();
        input->Update(dt);

        return Context::ConnectContext;
    }

    void ConnectContext::renderFunc()
    {
        input->Render();
    }
}


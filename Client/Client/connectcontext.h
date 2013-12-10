#ifndef CONNECTCONTEXT_H
#define CONNECTCONTEXT_H
#include <icontext.h>
#include "inputfield.h"

namespace NeonHockey
{
    struct ConnectContextData : public IContextData
    {
        ConnectContextData(int width, int height)
            : IContextData(width, height)
        { }
    };

    class ConnectContext : public IContext
    {
    public:
        ConnectContext(HGE* hge, ResourceManager* rm, ConnectContextData* data);
        ~ConnectContext();

        // IContext interface
    public:
        void show();
        IContextReturnData frameFunc();
        void renderFunc();

    private:
        hgeGUI *gui;

        InputField *ipField;
        InputField *nameField;
        hgeGUIButton *connectButton;

        enum ControlId { Ip = 1, Name = 2, Connect = 3};
    };
}

#endif // CONNECTCONTEXT_H

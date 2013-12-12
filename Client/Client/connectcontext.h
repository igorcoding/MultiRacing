#ifndef CONNECTCONTEXT_H
#define CONNECTCONTEXT_H
#include "icontext.h"
#include "inputfield.h"

#include <future>

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
        enum class ConnectionStatus
        {
            Default,
            Connecting,
            WaitingOpponent
        };

    public:
        ConnectContext(HGE* hge, std::shared_ptr<ResourceManager> rm, std::shared_ptr<ConnectContextData> data);
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

        bool connectToServer(const std::string& ip, const std::string& playerName);
        ConnectionStatus status;
        std::future<bool> connectionResult;
        template<typename R> bool is_ready(std::future<R>& f);
    };
}

#endif // CONNECTCONTEXT_H

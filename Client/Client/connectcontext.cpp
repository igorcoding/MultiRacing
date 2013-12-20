#include "connectcontext.h"
#include "ingamecontext.h"
#include "client.h"


namespace NeonHockey
{
    ConnectContext::ConnectContext(HGE* hge, std::shared_ptr<ResourceManager> rm, std::shared_ptr<ConnectContextData> data)
        : IContext(hge, rm, data),
          status(ConnectionStatus::Default)
    {
        gui = new hgeGUI();

        ipField = new InputField(_rm, ControlId::Ip, 100, 100, "127.0.0.1");
        nameField = new InputField(_rm, ControlId::Name, 100, 100 + 48, "Player0");

        HTEXTURE buttonTexture = _rm->getTexture(GfxType::BUTTON);
        connectButton = new hgeGUIButton(ControlId::Connect, 86, 190, 100, 28, buttonTexture, 0, 0);

        gui->AddCtrl(ipField);
        gui->AddCtrl(nameField);
        gui->AddCtrl(connectButton);

        statusStr = "Input server ip, your nickname and click Connect";
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
        //TODO: повторный запуск без сброса
        statusStr = "Input server ip, your nickname and click Connect";

        gui->Enter();
    }

    IContextReturnData ConnectContext::frameFunc()
    {
        float dt = _hge->Timer_GetDelta();
#ifdef SINGLEPLAYER_DEBUG
        if(gui->Update(dt) == ControlId::Connect)
        {
            return IContextReturnData(Context::InGameContext,
                                      std::make_shared<InGameContextData>(_data->screenWidth, _data->screenHeight, 0, "1", "2"));
        }
#else


        if(gui->Update(dt) == ControlId::Connect && status == ConnectionStatus::Default)
        {
            status = ConnectionStatus::Connecting;
            auto ip = std::string(ipField->getText());
            auto name = std::string(nameField->getText());
            connectionResult = std::async(std::launch::async, &ConnectContext::connectToServer, this, ip, name);
        }

        switch(status)
        {
        case ConnectionStatus::Connecting:
            if (connectionResult.valid() && is_ready(connectionResult))
            {
                bool connected = connectionResult.get();
                if (connected)
                {
                    statusStr = "Failed to connect. Please, reenter IP";
                    std::cout << statusStr << std::endl;
                    status = ConnectionStatus::WaitingOpponent;
                }
                else
                {
                    statusStr = "Failed to connect. Please, reenter IP";
                    std::cout << statusStr << std::endl;
                    status = ConnectionStatus::Default;
                }
            }
            break;
        case ConnectionStatus::WaitingOpponent:
            if(Client::getInstance().isGameStarted())
            {
                std::cout << "Waiting finished. Game started" << std::endl;
                auto playerId = Client::getInstance().id();
                auto opponentName = Client::getInstance().getOpponentName();
                return IContextReturnData(Context::InGameContext,
                                          std::make_shared<InGameContextData>(_data->screenWidth, _data->screenHeight, playerId, std::string(nameField->getText()), opponentName));
            }
            else
            {
                if(Client::getInstance().shouldStop())
                {
                    // clientThread.join(); //wait for it to die
                    status = ConnectionStatus::Default;
                }
            };
            break;
        default:
            break;
        }

#endif

        return IContextReturnData(Context::ConnectContext, _data);
    }

    void ConnectContext::renderFunc()
    {
        auto font = _rm->getFont(FontType::UI);

        font->Render(74, 100, HGETEXT_RIGHT, "IP: ");
        font->Render(74, 148, HGETEXT_RIGHT, "Name: ");

        gui->Render();

        font->Render(100, 100+48*2, HGETEXT_LEFT, "Connect");

        //TODO: move to StatusBar class
        font->Render(10, _data->screenHeight - font->GetHeight()*1.5,
                     HGETEXT_LEFT, statusStr.c_str());
    }

    bool ConnectContext::connectToServer(const std::string& ip, const std::string& playerName)
    {
        constexpr int port = 14882;
        try
        {
            bool connected = false;
            int connectionTries = 3;
            do
            {
                connected = Client::getInstance().connect(ip, port, playerName);
                --connectionTries;

            } while (!connected && connectionTries > 0);

            return connected;
        }
        catch(...)
        {
            return false;
        }
    }

    template<typename R>
    bool ConnectContext::is_ready(std::future<R>& f)
    {
        return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }
}


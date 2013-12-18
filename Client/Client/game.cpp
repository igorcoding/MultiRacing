#include "game.h"
#include "client.h"
#include "resourcesloader.h"
#include <cmath>
#include <cstdio>
#include <utility>

namespace NeonHockey
{
    Game::Game()
    { }

    bool Game::_initialized = false;
    const std::string Game::game_title = "Neon Hockey v0.1";
    const std::string Game::game_log = "neonhockey.log";
    const int Game::screen_width = 800;
    const int Game::screen_height = 600;

    HGE* Game::_hge = nullptr;
    std::shared_ptr<ResourceManager> Game::_resources = std::make_shared<ResourceManager>();
    Context Game::_currentContext = Context::MenuContext;
    std::map<Context, IContext*> Game::_contexts;


    void Game::start()
    {
        if (!_initialized)
            initializeGame();
        if (_initialized)
        {
            updateContext(Context::MenuContext, std::make_shared<MenuContextData>(screenWidth(), screenHeight()));
            _hge->System_Start();

            endGame();
        }
        
    }

    void Game::endGame()
    {
        _resources->freeResources();
        for (auto& context : _contexts)
            delete context.second;
        _currentContext = Context::NoContext;

        _hge->System_Shutdown();
        _hge->Release();

        _initialized = false;
    }

    int Game::screenWidth()
    {
        return screen_width;
    }

    int Game::screenHeight()
    {
        return screen_height;
    }

    Game::~Game()
    {
        if (_initialized)
            endGame();
    }

    void Game::initializeGame()
    {
        _hge = hgeCreate(HGE_VERSION);
        _resources->initHge(_hge);
        initializeGameStates();

        if (_hge->System_Initiate())
        {
            try
            {
                initializeGameResources();
                _initialized = true;
            }
            catch (std::exception& e)
            {
                endGame();
                throw; // forward throw
            }
        }


    }

    void Game::initializeGameStates()
    {
        _hge->System_SetState(HGE_LOGFILE, Game::game_log.c_str());
        _hge->System_SetState(HGE_FRAMEFUNC, Game::frameFunc);
        _hge->System_SetState(HGE_RENDERFUNC, Game::renderFunc);
        _hge->System_SetState(HGE_TITLE, Game::game_title.c_str());
        _hge->System_SetState(HGE_USESOUND, true);
        _hge->System_SetState(HGE_WINDOWED, true);
        _hge->System_SetState(HGE_HIDEMOUSE, false);
        _hge->System_SetState(HGE_SCREENWIDTH, Game::screen_width);
        _hge->System_SetState(HGE_SCREENHEIGHT, Game::screen_height);
        _hge->System_SetState(HGE_SCREENBPP, 32);
    }

    void Game::initializeGameResources()
    {
        ResourcesLoader rloader(_resources);
        rloader.loadEverything();
    }

    bool Game::frameFunc()
    {
        auto nextContext = currentContext()->frameFunc();
        updateContext(nextContext.c, nextContext.data);

        if (_currentContext == Context::NoContext)
            return true;

        return false;
    }

    bool Game::renderFunc()
    {
        if (_currentContext == Context::NoContext)
            return true;
        _hge->Gfx_BeginScene();
        _hge->Gfx_Clear(0);

        currentContext()->renderFunc();

        _hge->Gfx_EndScene();
        return false;
    }

    IContext* Game::context(Context c)
    {
        return _contexts[c];
    }

    IContext* Game::currentContext()
    {
        return _contexts[_currentContext];
    }

    void Game::updateContext(Context c, std::shared_ptr<IContextData> contextData)
    {
        auto context = _contexts.find(c);
        if (context == _contexts.end()) // did not find the context
        {
            switch(c)
            {
            case Context::MenuContext:
                _contexts[c] = new MenuContext(_hge, _resources, std::dynamic_pointer_cast<MenuContextData>(contextData));
                break;
            case Context::ConnectContext:
                _contexts[c] = new ConnectContext(_hge, _resources, std::dynamic_pointer_cast<ConnectContextData>(contextData));
                break;
            case Context::InGameContext:
                _contexts[c] = new InGameContext(_hge, _resources, std::dynamic_pointer_cast<InGameContextData>(contextData));
                break;
            case Context::GameOverContext:
                _contexts[c] = new GameOverContext(_hge, _resources, std::dynamic_pointer_cast<GameOverContextData>(contextData));
                break;
            case Context::GameErrorContext:
                break;
            case Context::NoContext:
                //endGame();
                break;
            default:
                break;
            }
        }
        else // found the context
        {
            context->second->changeData(contextData);
        }
        if (_currentContext != c && c != Context::NoContext)
            Game::context(c)->show();
        _currentContext = c;
    }

    void Game::initOptions()
    {
        //_options.insert(std::make_pair(OptionType::SCREEN_WIDTH, ))
    }

    void Game::playSound(SoundType type, int at, int volume)
    {
        HEFFECT snd = _resources->getSound(type);

        //converts at to [-100, 100] range
        _hge->Effect_PlayEx(snd, volume, at * 200 /screen_width - 100);
    }
}

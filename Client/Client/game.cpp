#include "game.h"
#include "client.h"
#include "resourcesloader.h"
#include <cmath>
#include <cstdio>

namespace NeonHockey
{
    Game::Game()
    { }

    int Game::_currentPlayerId = -1;
    bool Game::_initialized = false;
    const std::string Game::game_title = "Neon Hockey v0.1";
    const std::string Game::game_log = "neonhockey.log";
    const int Game::screen_width = 800;
    const int Game::screen_height = 600;

    HGE* Game::_hge = nullptr;
    ResourceManager Game::_resources;
    Context Game::_currentContext = Context::MenuContext;
    std::map<Context, IContext*> Game::_contexts;


    void Game::start()
    {
        if (!_initialized)
            initializeGame();
        if (_initialized)
        {
            updateContext(Context::MenuContext, new MenuContextData(screenWidth(), screenHeight()));
            _hge->System_Start();

            endGame();
        }
        
    }

    void Game::endGame()
    {
        _resources.freeResources();
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
        _resources.initHge(_hge);
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
        ResourcesLoader rloader(&_resources);
        rloader.loadEverything();
        /*
        try
        {
            _currentPlayerId = Client::getInstance().id();
            //int id = 0;
            BoardSide::BoardSide side0, side1;
            switch (_currentPlayerId)
            {
            case 0:
                side0 = BoardSide::LEFT;
                side1 = BoardSide::RIGHT;
                break;
            case 1:
                side0 = BoardSide::RIGHT;
                side1 = BoardSide::LEFT;
                break;
            default:
                throw std::exception();
            }

            // retrieve players
            int texture1_id = 2;
            int texture2_id = 2;

            Player p1("Player 1", side0, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture1_id])));
            Player p2("Player 2", side1, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture2_id])));

            _puck = std::move(std::unique_ptr<Puck>(new Puck(gfx_textures[1])));
            _players[_currentPlayerId] = std::move(p1);
            _players[!_currentPlayerId] = std::move(p2);

            _resources.addTexture(GfxType::BACKGROUND, gfx_textures[0].texturePath());
            _resources.addTexture(GfxType::PUCK, _puck->spriteInfo().texturePath());
            _resources.addTexture(GfxType::PADDLE_CURRENT, _players[_currentPlayerId].paddle()->spriteInfo().texturePath());
            _resources.addTexture(GfxType::PADDLE_ENEMY, _players[!_currentPlayerId].paddle()->spriteInfo().texturePath());


            _resources.addSprite(GfxType::BACKGROUND, gfx_textures[0])->SetHotSpot(0, 0);

            _resources.addSprite(GfxType::PUCK, _puck->spriteInfo())->SetHotSpot(_puck->spriteInfo().width() / 2,
                                                                                                  _puck->spriteInfo().height() / 2);

            _resources.addSprite(GfxType::PADDLE_CURRENT, _players[_currentPlayerId].paddle()->spriteInfo())->SetHotSpot(_players[_currentPlayerId].paddle()->spriteInfo().width() / 2,
                                                                                                              _players[_currentPlayerId].paddle()->spriteInfo().height() / 2);

            _resources.addSprite(GfxType::PADDLE_ENEMY, _players[!_currentPlayerId].paddle()->spriteInfo())->SetHotSpot(_players[!_currentPlayerId].paddle()->spriteInfo().width() / 2,
                                                                                                              _players[!_currentPlayerId].paddle()->spriteInfo().height() / 2);

            //_resources.addSound(SoundType::COLLISION, "../resources/hit");
            //_resources.addSound(SoundType::COLLISION, "/home/igor/Documents/projects/cpp/NeonHockey/Client/resources/hit.ogg");

            _resources.addFont(FontType::SCORE, "../resources/Digital.fnt");
        }
        catch(std::exception &e)
        {
            std::cerr << e.what();
            std::cerr << "Check game.log for details" << std::endl;

            //Client::getInstance().stop();
            //TODO: change state or close the game. maybe?
        }


        Client::getInstance().getPaddlePos(_players[_currentPlayerId].paddle()->x, _players[_currentPlayerId].paddle()->y);
        Client::getInstance().getEnemyPaddlePos(_players[!_currentPlayerId].paddle()->x, _players[!_currentPlayerId].paddle()->y);
        Client::getInstance().getPuckPos(_puck->x, _puck->y);
        */
    }

    bool Game::frameFunc()
    {
        auto nextContext = currentContext()->frameFunc();
        updateContext(nextContext.c, nextContext.data);


        return false;
    }

    bool Game::renderFunc()
    {
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

    void Game::updateContext(Context c, IContextData* contextData)
    {
        auto context = _contexts.find(c);
        if (context == _contexts.end()) // did not find the context
        {
            switch(c)
            {
            case Context::MenuContext:
                _contexts[c] = new MenuContext(_hge, &_resources, dynamic_cast<MenuContextData*>(contextData));
                break;
            case Context::ConnectContext:
                _contexts[c] = new ConnectContext(_hge, &_resources, dynamic_cast<ConnectContextData*>(contextData));
                break;
            case Context::InGameContext:
                _contexts[c] = new InGameContext(_hge, &_resources, dynamic_cast<InGameContextData*>(contextData));
                break;
            case Context::GameFinishedContext:
                break;
            case Context::GameErrorContext:
                break;
            default:
                break;
            }
        }
        else // found the context
        {
            context->second->changeData(contextData);
        }
        if (_currentContext != c)
            Game::context(c)->show();
        _currentContext = c;
    }

    void Game::playSound(SoundType::SoundObjectType type, int at, int volume)
    {
        HEFFECT snd = _resources.getSound(type);

        //converts at to [-100, 100] range
        _hge->Effect_PlayEx(snd, volume, at * 200 /screen_width - 100);
    }
}

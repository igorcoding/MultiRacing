#include "game.h"
#include "client.h"
#include <cmath>

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
    std::vector<SpriteInfo> Game::gfx_textures;

    HGE* Game::_hge = nullptr;
    std::vector<Player> Game::_players(2);
    Game::Puck_ptr Game::_puck = nullptr;
    ResourceManager Game::_resources;


/*
    Game& Game::getInstance()
    {
        static Game game;
        if (!game._initialized)
            initializeGame();
        return game;
    }
*/
    void Game::start()
    {
        if (!_initialized)
            initializeGame();
        if (_initialized)
        {
            _hge->System_Start();

            endGame();
        }
    }

    void Game::endGame()
    {
        _resources.freeResources();
        _hge->System_Shutdown();
        _hge->Release();

        _puck.reset();
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
        setGfxResources();
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

    void Game::setGfxResources()
    {
        Game::gfx_textures.emplace_back(SpriteInfo(96, 64, 32, 32, "../resources/particles.png"));
        Game::gfx_textures.emplace_back(SpriteInfo(0, 0, 64, 64, "../resources/zazaka.png"));
    }

    void Game::initializeGameStates()
    {
        _hge->System_SetState(HGE_LOGFILE, Game::game_log.c_str());
        _hge->System_SetState(HGE_FRAMEFUNC, Game::frameFunc);
        _hge->System_SetState(HGE_RENDERFUNC, Game::renderFunc);
        _hge->System_SetState(HGE_TITLE, Game::game_title.c_str());
        _hge->System_SetState(HGE_USESOUND, false);
        _hge->System_SetState(HGE_WINDOWED, true);
        _hge->System_SetState(HGE_HIDEMOUSE, false);
        _hge->System_SetState(HGE_SCREENWIDTH, Game::screen_width);
        _hge->System_SetState(HGE_SCREENHEIGHT, Game::screen_height);
        _hge->System_SetState(HGE_SCREENBPP, 32);
    }

    void Game::initializeGameResources()
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
        int texture1_id = 1;
        int texture2_id = 1;

        Player p1("Player 1", side0, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture1_id])));
        Player p2("Player 2", side1, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture2_id])));

        _puck = std::move(std::unique_ptr<Puck>(new Puck(gfx_textures[0])));
        //_players.emplace_back(std::move(p1));
        //_players.emplace_back(std::move(p2));
        _players[_currentPlayerId] = std::move(p1);
        _players[!_currentPlayerId] = std::move(p2);

        _resources.addTexture(GfxType::PUCK, _puck->spriteInfo().texturePath());
        _resources.addTexture(GfxType::PADDLE_CURRENT, _players[_currentPlayerId].paddle()->spriteInfo().texturePath());
        _resources.addTexture(GfxType::PADDLE_ENEMY, _players[!_currentPlayerId].paddle()->spriteInfo().texturePath());

        _resources.addSprite(GfxType::PUCK, _puck->spriteInfo().xTexturePos(),
                                              _puck->spriteInfo().yTexturePos(),
                                              _puck->spriteInfo().width(),
                                              _puck->spriteInfo().height())->SetHotSpot(_puck->spriteInfo().width() / 2,
                                                                                              _puck->spriteInfo().height() / 2);

        _resources.addSprite(GfxType::PADDLE_CURRENT, _players[_currentPlayerId].paddle()->spriteInfo().xTexturePos(),
                                                 _players[_currentPlayerId].paddle()->spriteInfo().yTexturePos(),
                                                 _players[_currentPlayerId].paddle()->spriteInfo().width(),
                                                 _players[_currentPlayerId].paddle()->spriteInfo().height())->SetHotSpot(_players[_currentPlayerId].paddle()->spriteInfo().width() / 2,
                                                                                                          _players[_currentPlayerId].paddle()->spriteInfo().height() / 2);

        _resources.addSprite(GfxType::PADDLE_ENEMY, _players[!_currentPlayerId].paddle()->spriteInfo().xTexturePos(),
                                                 _players[!_currentPlayerId].paddle()->spriteInfo().yTexturePos(),
                                                 _players[!_currentPlayerId].paddle()->spriteInfo().width(),
                                                 _players[!_currentPlayerId].paddle()->spriteInfo().height())->SetHotSpot(_players[!_currentPlayerId].paddle()->spriteInfo().width() / 2,
                                                                                                          _players[!_currentPlayerId].paddle()->spriteInfo().height() / 2);


        Client::getInstance().getPaddlePos(_players[_currentPlayerId].paddle()->x, _players[_currentPlayerId].paddle()->y);
        Client::getInstance().getEnemyPaddlePos(_players[!_currentPlayerId].paddle()->x, _players[!_currentPlayerId].paddle()->y);
        Client::getInstance().getPuckPos(_puck->x, _puck->y);

        /*
        // initial rendering values
        const float x_offset = 50.0f;
        _puck->x = screen_width / 2;
        _puck->y = screen_height / 2;
        for (size_t i = 0; i < _players.size(); ++i)
        {
            _players[i].paddle()->x = fabs((int) _players[i].getSide() * screen_width - x_offset);
            _players[i].paddle()->y = screen_height / 2;
        }
        */
    }

    bool Game::frameFunc()
    {
        auto dt = _hge->Timer_GetDelta();

        Player& currentPlayer = _players[_currentPlayerId];
        Player& enemyPlayer = _players[!_currentPlayerId];

        float mouse_x = 0;
        float mouse_y = 0;

        if (_hge->Input_IsMouseOver() && _hge->Input_GetKeyState(HGEK_LBUTTON))
        {

            _hge->Input_GetMousePos(&mouse_x, &mouse_y);
            bool inplace = true;
            switch (_players[_currentPlayerId].getSide())
            {
            case BoardSide::LEFT:
                if (!(mouse_x <= screen_width / 2 - currentPlayer.paddle()->spriteInfo().width() / 2))
                    inplace = false;
                break;
            case BoardSide::RIGHT:
                if (!(mouse_x >= screen_width / 2 + currentPlayer.paddle()->spriteInfo().width() / 2))
                    inplace = false;
                break;
            }

            if (inplace)
            {
                currentPlayer.paddle()->x = mouse_x;
                currentPlayer.paddle()->y = mouse_y;
                Client::getInstance().sendPaddlePos(currentPlayer.paddle()->x, currentPlayer.paddle()->y);
            }

        }

        //updae coords from server
        Client::getInstance().getEnemyPaddlePos(enemyPlayer.paddle()->x, enemyPlayer.paddle()->y);

        Client::getInstance().getPuckPos(_puck->x, _puck->y);

        //some kind of error occured?
        if(Client::getInstance().shouldStop())
            return true;

        return false;
    }

    bool Game::renderFunc()
    {
        _hge->Gfx_BeginScene();
        _hge->Gfx_Clear(0);

        auto puckSprite = _resources.getSprite(GfxType::PUCK);
        auto paddleSprite0 = _resources.getSprite(GfxType::PADDLE_CURRENT);
        auto paddleSprite1 = _resources.getSprite(GfxType::PADDLE_ENEMY);

        puckSprite->Render(_puck->x, _puck->y);
        paddleSprite0->Render(_players[0].paddle()->x, _players[0].paddle()->y);
        paddleSprite1->Render(_players[1].paddle()->x, _players[1].paddle()->y);

        _hge->Gfx_EndScene();
        return false;
    }


}

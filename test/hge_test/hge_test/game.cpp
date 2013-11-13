#include "game.h"
#include <cmath>

namespace NeonHockey
{
    Game::Game()
        : _initialized(false)
    { }

    const std::string Game::game_title = "Neon Hockey v0.1";
    const std::string Game::game_log = "neonhockey.log";
    const int Game::screen_width = 800;
    const int Game::screen_height = 600;
    std::vector<SpriteInfo> Game::gfx_textures;

    HGE* Game::_hge = nullptr;
    std::vector<Player> Game::_players;
    Game::Puck_ptr Game::_puck;
    ResourceManager Game::_resources;



    Game& Game::getInstance()
    {
        static Game game;
        if (!game._initialized)
            initializeGame(&game);
        return game;
    }

    void Game::start()
    {
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

    Game::~Game()
    {
        if (_initialized)
            endGame();
    }

    void Game::initializeGame(Game* game)
    {
        game->_hge = hgeCreate(HGE_VERSION);
        game->_resources.initHge(game->_hge);
        setGfxResources();
        initializeGameStates(game);

        if (game->_hge->System_Initiate())
        {
            try
            {
                initializeGameResources(game);
                game->_initialized = true;
            }
            catch (std::exception& e)
            {
                game->endGame();
                throw e; // forward throw
            }
        }


    }

    void Game::setGfxResources()
    {
        Game::gfx_textures.emplace_back(SpriteInfo(96, 64, 32, 32, "resources/particles.png"));
        Game::gfx_textures.emplace_back(SpriteInfo(0, 0, 64, 64, "resources/zazaka.png"));
    }

    void Game::initializeGameStates(Game* game)
    {
        game->_hge->System_SetState(HGE_LOGFILE, Game::game_log.c_str());
        game->_hge->System_SetState(HGE_FRAMEFUNC, Game::frameFunc);
        game->_hge->System_SetState(HGE_RENDERFUNC, Game::renderFunc);
        game->_hge->System_SetState(HGE_TITLE, Game::game_title.c_str());
        game->_hge->System_SetState(HGE_USESOUND, false);
        game->_hge->System_SetState(HGE_WINDOWED, true);
        game->_hge->System_SetState(HGE_HIDEMOUSE, false);
        game->_hge->System_SetState(HGE_SCREENWIDTH, Game::screen_width);
        game->_hge->System_SetState(HGE_SCREENHEIGHT, Game::screen_height);
        game->_hge->System_SetState(HGE_SCREENBPP, 32);
    }

    void Game::initializeGameResources(Game* game)
    {
        // retrieve players
        int texture1_id = 1;
        int texture2_id = 1;

        Player p1("Player 1", BoardSide::RIGHT, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture1_id])));
        Player p2("Player 2", BoardSide::LEFT, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture2_id])));

        _puck = std::move(std::unique_ptr<Puck>(new Puck(gfx_textures[0])));
        _players.emplace_back(std::move(p1));
        _players.emplace_back(std::move(p2));

        _resources.addTexture(GfxType::PUCK, game->_puck->spriteInfo().texturePath());
        _resources.addTexture(GfxType::PADDLE1, game->_players[0].paddle()->spriteInfo().texturePath());
        _resources.addTexture(GfxType::PADDLE2, game->_players[1].paddle()->spriteInfo().texturePath());


        _resources.addSprite(GfxType::PUCK, game->_puck->spriteInfo().xTexturePos(),
                                              game->_puck->spriteInfo().yTexturePos(),
                                              game->_puck->spriteInfo().width(),
                                              game->_puck->spriteInfo().height())->SetHotSpot(game->_puck->spriteInfo().width() / 2,
                                                                                              game->_puck->spriteInfo().height() / 2);

        _resources.addSprite(GfxType::PADDLE1, game->_players[0].paddle()->spriteInfo().xTexturePos(),
                                                 game->_players[0].paddle()->spriteInfo().yTexturePos(),
                                                 game->_players[0].paddle()->spriteInfo().width(),
                                                 game->_players[0].paddle()->spriteInfo().height())->SetHotSpot(game->_players[0].paddle()->spriteInfo().width() / 2,
                                                                                                                game->_players[0].paddle()->spriteInfo().height() / 2);

        _resources.addSprite(GfxType::PADDLE2, game->_players[1].paddle()->spriteInfo().xTexturePos(),
                                                 game->_players[1].paddle()->spriteInfo().yTexturePos(),
                                                 game->_players[1].paddle()->spriteInfo().width(),
                                                 game->_players[1].paddle()->spriteInfo().height())->SetHotSpot(game->_players[1].paddle()->spriteInfo().width() / 2,
                                                                                                                game->_players[1].paddle()->spriteInfo().height() / 2);


        // inittial rendering values
        const float x_offset = 50.0f;
        _puck->x = screen_width / 2;
        _puck->y = screen_height / 2;
        for (size_t i = 0; i < _players.size(); ++i)
        {
            _players[i].paddle()->x = fabs((int) _players[i].getSide() * screen_width - x_offset);
            _players[i].paddle()->y = screen_height / 2;
        }

    }



    bool Game::frameFunc()
    {
        auto dt = _hge->Timer_GetDelta();

        float mouse_x = 0;
        float mouse_y = 0;

        if (_hge->Input_IsMouseOver() && _hge->Input_GetKeyState(HGEK_LBUTTON))
        {
            _hge->Input_GetMousePos(&mouse_x, &mouse_y);
            bool inplace = false;
            switch (_players[0].getSide())
            {
            case BoardSide::LEFT:
                if (mouse_x <= screen_width / 2)
                    inplace = true;
                break;
            case BoardSide::RIGHT:
                if (mouse_x >= screen_width / 2)
                    inplace = true;
                break;
            }

            if (inplace)
            {
                _players[0].paddle()->x = mouse_x;
                _players[0].paddle()->y = mouse_y;
            }

        }



        return false;
    }

    bool Game::renderFunc()
    {
        _hge->Gfx_BeginScene();
        _hge->Gfx_Clear(0);

        auto puckSprite = _resources.getSprite(GfxType::PUCK);
        auto paddleSprite1 = _resources.getSprite(GfxType::PADDLE1);
        auto paddleSprite2 = _resources.getSprite(GfxType::PADDLE2);

        puckSprite->Render(_puck->x, _puck->y);
        paddleSprite1->Render(_players[0].paddle()->x, _players[0].paddle()->y);
        paddleSprite2->Render(_players[1].paddle()->x, _players[1].paddle()->y);

        _hge->Gfx_EndScene();
        return false;
    }


}

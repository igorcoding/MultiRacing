#include "game.h"

namespace NeonHockey
{
    Game::Game()
        : _initialized(false)
    { }

    const std::string Game::game_title = "Neon Hockey v0.1";
    const std::string Game::game_log = "neonhockey.log";
    const int Game::screen_width = 800; //REVIEW: давай ориентировать на 16:9, например на ноутбучные 1366x768
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
                throw; // forward throw
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

        //const SpriteInfo& tex1 = gfx_textures[texture1_id];
        Player p1("Player 1", BoardSide::TOP, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture1_id])));
        Player p2("Player 2", BoardSide::BOTTOM, 0, std::unique_ptr<Paddle>(new Paddle(gfx_textures[texture2_id])));

        _puck = std::move(std::unique_ptr<Puck>(new Puck(gfx_textures[0])));
        _players.emplace_back(std::move(p1));
        _players.emplace_back(std::move(p2));

        _resources.addTexture(GfxType::PUCK, game->_puck->spriteInfo().texturePath());
        _resources.addTexture(GfxType::PADDLE1, game->_players[0].paddle()->spriteInfo().texturePath());
        _resources.addTexture(GfxType::PADDLE2, game->_players[1].paddle()->spriteInfo().texturePath());


        _resources.addSprite(GfxType::PUCK, game->_puck->spriteInfo().xTexturePos(),
                                              game->_puck->spriteInfo().yTexturePos(),
                                              game->_puck->spriteInfo().width(),
                                              game->_puck->spriteInfo().height());

        _resources.addSprite(GfxType::PADDLE1, game->_players[0].paddle()->spriteInfo().xTexturePos(),
                                                 game->_players[0].paddle()->spriteInfo().yTexturePos(),
                                                 game->_players[0].paddle()->spriteInfo().width(),
                                                 game->_players[0].paddle()->spriteInfo().height());

        _resources.addSprite(GfxType::PADDLE2, game->_players[1].paddle()->spriteInfo().xTexturePos(),
                                                 game->_players[1].paddle()->spriteInfo().yTexturePos(),
                                                 game->_players[1].paddle()->spriteInfo().width(),
                                                 game->_players[1].paddle()->spriteInfo().height());
    }



    bool Game::frameFunc()
    {
        return false;
    }

    bool Game::renderFunc()
    {
        _hge->Gfx_BeginScene();
        _hge->Gfx_Clear(0);


        auto puckSprite = _resources.getSprite(GfxType::PUCK);
        auto paddleSprite1 = _resources.getSprite(GfxType::PADDLE1);
        auto paddleSprite2 = _resources.getSprite(GfxType::PADDLE2);

        puckSprite->Render(100.0, 100.0);
        paddleSprite1->Render(200.0, 200.0);
        paddleSprite2->Render(300.0, 300.0);

        _hge->Gfx_EndScene();
        return false;
    }


}

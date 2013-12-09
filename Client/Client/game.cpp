#include "game.h"
#include "client.h"
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
    const int Game::border_width = 32;
    const int Game::gap_width = 200;
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
        Game::gfx_textures.emplace_back(SpriteInfo(0, 0, 800, 600, "../resources/Field.png"));
        Game::gfx_textures.emplace_back(SpriteInfo(0, 0, 64, 64, "../resources/Puck.png"));
        Game::gfx_textures.emplace_back(SpriteInfo(0, 0, 64, 64, "../resources/Paddle.png"));

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

    void Game::initializeGameResources() //REVIEW: add try/catch block over all loadings
    {
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
    }

    bool Game::frameFunc()
    {
        //auto dt = _hge->Timer_GetDelta();

        Player& currentPlayer = _players[_currentPlayerId];
        Player& enemyPlayer = _players[!_currentPlayerId];

        float mouse_x = 0;
        float mouse_y = 0;

        if (_hge->Input_IsMouseOver() && _hge->Input_GetKeyState(HGEK_LBUTTON))
        {
            _hge->Input_GetMousePos(&mouse_x, &mouse_y);
            bool inplace = true;

            auto x_max = screenWidth() - border_width - currentPlayer.paddle()->spriteInfo().width() / 2;
            auto x_min = border_width + currentPlayer.paddle()->spriteInfo().width() / 2;
            auto y_max = screenHeight() - border_width - currentPlayer.paddle()->spriteInfo().height() / 2;
            auto y_min = border_width + currentPlayer.paddle()->spriteInfo().height() / 2;

            if (mouse_x > x_max || mouse_x < x_min || mouse_y > y_max || mouse_y < y_min)
            {
                inplace = false;
            }

            if (mouse_y > (screenHeight() - gap_width) / 2 + currentPlayer.paddle()->spriteInfo().height() / 2 &&
                    mouse_y < (screenHeight() + gap_width) / 2 - currentPlayer.paddle()->spriteInfo().height() / 2)
            {
                inplace = true;
                if (mouse_x > screenWidth() - currentPlayer.paddle()->spriteInfo().width() ||
                        mouse_x < currentPlayer.paddle()->spriteInfo().width())
                    inplace = false;
            }
            


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
        //Client::getInstance().sendPaddlePos(currentPlayer.paddle()->x, currentPlayer.paddle()->y);

        //update coords from server
        Client::getInstance().getEnemyPaddlePos(enemyPlayer.paddle()->x, enemyPlayer.paddle()->y);

        Client::getInstance().getPuckPos(_puck->x, _puck->y);



        //sounds system

        //test for collisions
        int x = 0;
        int force = 0;
        //if(Client::getInstance().getCollision(x, force))
        //    playSound(SoundType::COLLISION, x, force);

        //some kind of error occured?
        if(Client::getInstance().shouldStop())
            return true;

        return false;
    }

    bool Game::renderFunc()
    {
        _hge->Gfx_BeginScene();
        _hge->Gfx_Clear(0);

        try
        {
            auto bgSprite = _resources.getSprite(GfxType::BACKGROUND);
            auto puckSprite = _resources.getSprite(GfxType::PUCK);
            auto paddleSprite0 = _resources.getSprite(GfxType::PADDLE_CURRENT);
            auto paddleSprite1 = _resources.getSprite(GfxType::PADDLE_ENEMY);

            bgSprite->Render(0, 0);
            puckSprite->Render(_puck->x, _puck->y);
            paddleSprite0->Render(_players[0].paddle()->x, _players[0].paddle()->y);
            paddleSprite1->Render(_players[1].paddle()->x, _players[1].paddle()->y);

            //render scores
            hgeFont *fnt = _resources.getFont(FontType::SCORE);
            fnt->SetColor(ARGB(100, 255, 100, 100));
            int scoreLeft = _players[0].getPoints();
            int scoreRight = _players[1].getPoints();

            //TODO: font does load, but doesn't rendering at all
            fnt->printf(0, 100, HGETEXT_LEFT, "%d : %d", scoreLeft, scoreRight);
        }
        catch(std::exception &e)
        {
            std::cerr << e.what();
            std::cerr << "Check game.log for details" << std::endl;

            Client::getInstance().stop();
            //TODO: change state or close the game. maybe?
        }

        _hge->Gfx_EndScene();

        return false;
    }

    void Game::playSound(SoundType::SoundObjectType type, int at, int volume)
    {
        HEFFECT snd = _resources.getSound(type);

        //converts at to [-100, 100] range
        _hge->Effect_PlayEx(snd, volume, at * 200 /screen_width - 100);
    }
}

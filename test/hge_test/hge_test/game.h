#ifndef GAME_H
#define GAME_H

#include <unix_platform.h>
#include <hge.h>

#include <vector>
#include <memory>

#include "resourcemanager.h"
#include "puck.h"
#include "paddle.h"

namespace NeonHockey
{
    class Game
    {
        using Puck_ptr = std::unique_ptr<Puck>;
    public:
        static Game& getInstance();
        void start();
        void endGame();
        ~Game();



    private:
        static HGE* _hge;
        static std::vector<Player> _players;
        static Puck_ptr _puck;
        static ResourceManager _resources;
        bool _initialized;

        static const std::string game_title;
        static const std::string game_log;
        static const int screen_width;
        static const int screen_height;
        static std::vector<SpriteInfo> gfx_textures;

        Game();
        Game(const Game&);
        Game& operator =(const Game&);

        static void initializeGame(Game* game);
        static void setGfxResources();
        static void initializeGameStates(Game* game);
        static void initializeGameResources(Game* game);
        static bool frameFunc();
        static bool renderFunc();

    };

}

#endif // GAME_H

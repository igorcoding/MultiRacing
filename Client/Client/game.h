#ifndef GAME_H
#define GAME_H

#include <hge.h>
#include <boost/any.hpp>
#include <boost/variant.hpp>

#include <vector>
#include <memory>

#include "resourcemanager.h"
#include "icontext.h"
#include "menucontext.h"
#include "ingamecontext.h"
#include "connectcontext.h"

namespace NeonHockey
{
    class Game
    {
    public:
        static void start();
        static void endGame();

        static int screenWidth();
        static int screenHeight();
        static const char* gameTitle();
        static const char* gameLog();


        ~Game();

    private:
        static HGE* _hge;
        static ResourceManager _resources;
        static bool _initialized;
        static std::map<Context, IContext*> _contexts;
        static Context _currentContext;

        static const std::string game_title;
        static const std::string game_log;
        static const int screen_width;
        static const int screen_height;

        Game();
        Game(const Game&) = delete;
        Game& operator =(const Game&) = delete;

        static void initializeGame();
        static void initializeGameStates();
        static void initializeGameResources();
        static bool frameFunc();
        static bool renderFunc();

        static IContext* context(Context c);
        static IContext* currentContext();
        static void updateContext(Context c, IContextData* contextData);


        static void initOptions();
        static bool isWindowed();
        static bool isUsingSound();
        static bool isMouseHidden();
        static int screenBpp();
        static void playSound(SoundType type, int at = 0, int volume = 100);

    };

}

#endif // GAME_H

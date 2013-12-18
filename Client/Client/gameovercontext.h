#ifndef GAMEOVERCONTEXT_H
#define GAMEOVERCONTEXT_H
#include "icontext.h"
#include "player.h"

namespace NeonHockey
{
    struct GameOverContextData : public IContextData
    {
        GameOverContextData(int width, int height, bool win, Player *leftPlayer, Player *rightPlayer)
            : IContextData(width, height),
              _win(win),
              _leftPlayer(leftPlayer),
              _rightPlayer(rightPlayer)
        { }

        bool _win;
        Player *_leftPlayer;
        Player *_rightPlayer;
    };

    class GameOverContext : public IContext
    {
    public:
        GameOverContext(HGE* hge, std::shared_ptr<ResourceManager> rm, std::shared_ptr<GameOverContextData> data);

        void show();
        IContextReturnData frameFunc();
        void renderFunc();
    };
}
#endif // GAMEOVERCONTEXT_H

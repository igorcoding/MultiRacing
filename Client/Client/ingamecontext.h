#ifndef INGAMECONTEXT_H
#define INGAMECONTEXT_H

#include <vector>
#include <memory>

#include "icontext.h"
#include "puck.h"
#include "player.h"
#include "spriteinfo.h"

namespace NeonHockey
{
    struct InGameContextData : public IContextData
    {
        InGameContextData(int width, int height, int currentPlayerId, int currentPlayerTexture, int enemyPlayerTexture)
            : IContextData(width, height),
              currentPlayerId(currentPlayerId),
              currentPlayerTexture(currentPlayerTexture),
              enemyPlayerTexture(enemyPlayerTexture)
        { }

        int currentPlayerId;
        int currentPlayerTexture;
        int enemyPlayerTexture;
    };

    class InGameContext : public IContext
    {
        using Puck_ptr = std::unique_ptr<Puck>;
    public:
        InGameContext(HGE* hge, ResourceManager* rm, InGameContextData* data);
        ~InGameContext() {}

        void show();
        IContextReturnData frameFunc();
        void renderFunc();


    private:

        std::vector<Player> _players;
        Puck_ptr _puck;
        const int border_width;
        const int gap_width;
    };

}

#endif // INGAMECONTEXT_H
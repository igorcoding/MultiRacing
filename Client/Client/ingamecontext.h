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

        virtual void start();
        virtual void show();
        virtual Context frameFunc();
        virtual void renderFunc();


    private:

        std::vector<Player> _players;
        Puck_ptr _puck;
        const int border_width;
        const int gap_width;
    };

}

#endif // INGAMECONTEXT_H

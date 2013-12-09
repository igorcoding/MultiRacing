#ifndef INGAMECONTEXT_H
#define INGAMECONTEXT_H

#include <vector>
#include <memory>

#include "icontext.h"
#include "puck.h"
#include "player.h"

namespace NeonHockey
{
    struct InGameContextData : public IContextData
    {
        int currentPlayerId;
    };

    class InGameContext : public IContext
    {
        using Puck_ptr = std::unique_ptr<Puck>;
    public:
        InGameContext(HGE* hge, ResourceManager* rm, InGameContextData* data);

        virtual void frameFunc();
        virtual void renderFunc();
        virtual void show();

    private:
        std::vector<Player> _players;
        Puck_ptr _puck;
        const int border_width;
        const int gap_width;
    };

}

#endif // INGAMECONTEXT_H

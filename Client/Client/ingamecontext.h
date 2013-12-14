#ifndef INGAMECONTEXT_H
#define INGAMECONTEXT_H

#include <vector>
#include <memory>

#include "icontext.h"
#include "puck.h"
#include "player.h"
#include "spriteinfo.h"
#include "timerfactory.h"

namespace NeonHockey
{
    struct InGameContextData : public IContextData
    {
        InGameContextData(int width, int height, int currentPlayerId, const std::string& currentName, const std::string& opponentName)
            : IContextData(width, height),
              currentPlayerId(currentPlayerId),
              currentPlayerName(currentName),
              opponentName(opponentName)
        { }

        int currentPlayerId;
        std::string currentPlayerName;
        std::string opponentName;
    };

    class InGameContext : public IContext
    {
        using Puck_ptr = std::unique_ptr<Puck>;
    public:
        InGameContext(HGE* hge, std::shared_ptr<ResourceManager> rm, std::shared_ptr<InGameContextData> data);
        ~InGameContext() {}

        void show();
        IContextReturnData frameFunc();
        void renderFunc();


    private:        
        TimerFactory timers;

        std::vector<Player> _players;
        Puck_ptr _puck;
        const int lr_border;
        const int tb_border;
        const int gap_width;
        int lastGoalSide = 0; //0 - left, 1 - right
    };

}

#endif // INGAMECONTEXT_H

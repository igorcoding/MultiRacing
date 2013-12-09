#include "ingamecontext.h"
#include "client.h"

namespace NeonHockey
{
    InGameContext::InGameContext(HGE* hge, ResourceManager* rm, InGameContextData* data)
        : IContext(hge, rm, data),
          border_width(32),
          gap_width(200)
    {
    }

    void InGameContext::frameFunc()
    {
        Player& currentPlayer = _players[_currentPlayerId];
        Player& enemyPlayer = _players[!_currentPlayerId];

        float mouse_x = 0;
        float mouse_y = 0;

        if (_hge->Input_IsMouseOver() && _hge->Input_GetKeyState(HGEK_LBUTTON))
        {
            _hge->Input_GetMousePos(&mouse_x, &mouse_y);
            bool inplace = true;

            auto x_max = _data->screenWidth - border_width - currentPlayer.paddle()->spriteInfo().width() / 2;
            auto x_min = border_width + currentPlayer.paddle()->spriteInfo().width() / 2;
            auto y_max = _data->screenHeight - border_width - currentPlayer.paddle()->spriteInfo().height() / 2;
            auto y_min = border_width + currentPlayer.paddle()->spriteInfo().height() / 2;

            if (mouse_x > x_max || mouse_x < x_min || mouse_y > y_max || mouse_y < y_min)
            {
                inplace = false;
            }

            if (mouse_y > (_data->screenHeight - gap_width) / 2 + currentPlayer.paddle()->spriteInfo().height() / 2 &&
                    mouse_y < (_data->screenHeight + gap_width) / 2 - currentPlayer.paddle()->spriteInfo().height() / 2)
            {
                inplace = true;
                if (mouse_x > _data->screenWidth - currentPlayer.paddle()->spriteInfo().width() ||
                        mouse_x < currentPlayer.paddle()->spriteInfo().width())
                    inplace = false;
            }



            switch (_players[_currentPlayerId].getSide())
            {
            case BoardSide::LEFT:
                if (!(mouse_x <= _data->screenWidth / 2 - currentPlayer.paddle()->spriteInfo().width() / 2))
                    inplace = false;
                break;
            case BoardSide::RIGHT:
                if (!(mouse_x >= _data->screenHeight / 2 + currentPlayer.paddle()->spriteInfo().width() / 2))
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

    void InGameContext::renderFunc()
    {
    }


}

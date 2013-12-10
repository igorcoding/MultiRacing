#include <sstream>
#include "ingamecontext.h"
#include "client.h"
#include "resourcesloader.h"

namespace NeonHockey
{
    InGameContext::InGameContext(HGE* hge, ResourceManager* rm, InGameContextData* data)
        : IContext(hge, rm, data),
          border_width(32),
          gap_width(200)
    {
    }

    void InGameContext::show()
    {
        renderFunc();
    }

    IContextReturnData InGameContext::frameFunc()
    {
        auto data = dynamic_cast<InGameContextData*>(_data);

        Player& currentPlayer = _players[data->currentPlayerId];
        Player& enemyPlayer = _players[!data->currentPlayerId];

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

            if (mouse_y > (data->screenHeight - gap_width) / 2 + currentPlayer.paddle()->spriteInfo().height() / 2 &&
                    mouse_y < (data->screenHeight + gap_width) / 2 - currentPlayer.paddle()->spriteInfo().height() / 2)
            {
                inplace = true;
                if (mouse_x > data->screenWidth - currentPlayer.paddle()->spriteInfo().width() ||
                        mouse_x < currentPlayer.paddle()->spriteInfo().width())
                    inplace = false;
            }



            switch (_players[data->currentPlayerId].getSide())
            {
            case BoardSide::LEFT:
                if (!(mouse_x <= data->screenWidth / 2 - currentPlayer.paddle()->spriteInfo().width() / 2))
                    inplace = false;
                break;
            case BoardSide::RIGHT:
                if (!(mouse_x >= data->screenHeight / 2 + currentPlayer.paddle()->spriteInfo().width() / 2))
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
            return IContextReturnData(Context::GameErrorContext, data);

        return IContextReturnData(Context::InGameContext, data);
    }

    void InGameContext::renderFunc()
    {


        try
        {
            auto bgSprite = _rm->getSprite(GfxType::BACKGROUND);
            auto puckSprite = _rm->getSprite(GfxType::PUCK);
            auto paddleSprite = _rm->getSprite(GfxType::PADDLE);

            bgSprite->Render(0, 0);
            puckSprite->Render(_puck->x, _puck->y);
            paddleSprite->Render(_players[0].paddle()->x, _players[0].paddle()->y);
            paddleSprite->Render(_players[1].paddle()->x, _players[1].paddle()->y);

            //render scores
            hgeFont *fnt = _rm->getFont(FontType::SCORE);
            fnt->SetColor(ARGB(255, 200, 200, 255));

            std::stringstream scoresStr;
            scoresStr << "(" << _players[0].getName() << ") "
                    << _players[0].getPoints()
                    << " : "
                    << _players[1].getPoints()
                    << "(" + _players[1].getName() << ")";


            float x = _data->screenWidth/2 - fnt->GetStringWidth(scoresStr.str().c_str());
            float y = fnt->GetHeight() * 1.5;

            fnt->Render(x, y, HGETEXT_LEFT, scoresStr.str().c_str());
        }
        catch(std::exception &e)
        {
            std::cerr << e.what();
            std::cerr << "Check game.log for details" << std::endl;

            //Client::getInstance().stop();
            //TODO: change state or close the game. maybe?
        }



        //return false;
    }



}



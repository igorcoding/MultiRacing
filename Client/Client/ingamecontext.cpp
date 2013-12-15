#include <sstream>
#include "ingamecontext.h"
#include "client.h"
#include "resourcesloader.h"

namespace NeonHockey
{
    InGameContext::InGameContext(HGE *hge, std::shared_ptr<ResourceManager> rm, std::shared_ptr<InGameContextData> gameData)
        : IContext(hge, rm, gameData),
          _players(2),
          _puck(nullptr),
          lr_border(26),
          tb_border(30),
          gap_width(200),
          timeoutTimer(10.0 / 1000.0, false,
                       [this](float dt)
                        {
                            std::cout << "timeout\n";
                            auto data = std::dynamic_pointer_cast<InGameContextData>(_data);
                            Player& currentPlayer = _players[data->currentPlayerId];
                            Client::getInstance().sendPaddlePos(currentPlayer.paddle()->x,
                                                                currentPlayer.paddle()->y, true);
                        })

    {
        auto data = std::dynamic_pointer_cast<InGameContextData>(_data);

        BoardSide::BoardSide side0, side1;
        switch (data->currentPlayerId)
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

        Player p0(data->currentPlayerName, side0, 0, std::unique_ptr<Paddle>(new Paddle(_rm->getSprite(GfxType::PADDLE))));
        Player p1(data->opponentName, side1, 0, std::unique_ptr<Paddle>(new Paddle(_rm->getSprite(GfxType::PADDLE))));

        _puck = std::move(std::unique_ptr<Puck>(new Puck(_rm->getSprite(GfxType::PUCK))));
        _players[data->currentPlayerId] = std::move(p0);
        _players[!data->currentPlayerId] = std::move(p1);

        float paddle_x = -1, paddle_y = -1;
        float enemy_x = -1, enemy_y = -1;
        float puck_x = -1, puck_y = -1;
        Client::getInstance().getPaddlePos(paddle_x, paddle_y);
        Client::getInstance().getEnemyPaddlePos(enemy_x, enemy_y);
        Client::getInstance().getPuckPos(puck_x, puck_y);

        _players[data->currentPlayerId].paddle()->setInitPos(paddle_x, paddle_y);
        _players[!data->currentPlayerId].paddle()->setInitPos(enemy_x, enemy_y);
        _puck->setInitPos(puck_x, puck_y);

    }

    void InGameContext::show()
    {

    }

    IContextReturnData InGameContext::frameFunc()
    {
        float dt = _hge->Timer_GetDelta();
        auto data = std::dynamic_pointer_cast<InGameContextData>(_data);

        Player& currentPlayer = _players[data->currentPlayerId];
        Player& enemyPlayer = _players[!data->currentPlayerId];

        if (_hge->Input_IsMouseOver() && _hge->Input_GetKeyState(HGEK_LBUTTON))
        {
            timeoutTimer.stop();
            float mouse_x = 0;
            float mouse_y = 0;
            _hge->Input_GetMousePos(&mouse_x, &mouse_y);
            checkAllowedBounds(mouse_x, mouse_y);

            currentPlayer.paddle()->x = mouse_x;
            currentPlayer.paddle()->y = mouse_y;
            Client::getInstance().sendPaddlePos(currentPlayer.paddle()->x, currentPlayer.paddle()->y);
            timeoutTimer.start();
        }
        Client::getInstance().getEnemyPaddlePos(enemyPlayer.paddle()->x, enemyPlayer.paddle()->y);
        Client::getInstance().getPuckPos(_puck->x, _puck->y);

        checkCollisions();
        checkGoal();

        timers.update(dt);
        timeoutTimer.update(dt);


        if(Client::getInstance().shouldStop())
            return IContextReturnData(Context::GameErrorContext, data);

        return IContextReturnData(Context::InGameContext, data);
    }

    void InGameContext::renderFunc()
    {
        try
        {
            float dt = _hge->Timer_GetDelta();

            auto bgSprite = _rm->getSprite(GfxType::BACKGROUND);
            auto puckSprite = _rm->getSprite(GfxType::PUCK);
            auto paddleSprite = _rm->getSprite(GfxType::PADDLE);

            bgSprite->Render(0, 0);
            puckSprite->Render(_puck->x, _puck->y);
            paddleSprite->Render(_players[0].paddle()->x, _players[0].paddle()->y);
            paddleSprite->Render(_players[1].paddle()->x, _players[1].paddle()->y);

            //render scores
            auto fnt = _rm->getFont(FontType::SCORE);
            fnt->SetColor(ARGB(255, 200, 200, 255));

            std::stringstream scoresStr;
            scoresStr << "(" << _players[0].getName() << ")   "
                    << _players[0].getPoints()
                    << " ";

            float widthBeforeColon = fnt->GetStringWidth(scoresStr.str().c_str());

            scoresStr << ": " << _players[1].getPoints()
                    << "   (" + _players[1].getName() << ")";


            float x = _data->screenWidth / 2 - widthBeforeColon;
            float y = fnt->GetHeight() / 2;

            fnt->Render(x, y, HGETEXT_LEFT, scoresStr.str().c_str());

            timers.render(dt);
        }
        catch(std::exception &e)
        {
            std::cerr << e.what();
            std::cerr << "Check game.log for details" << std::endl;

            //Client::getInstance().stop();
            //TODO: change state or close the game. maybe?
        }
    }

    void InGameContext::checkAllowedBounds(float& mouse_x, float& mouse_y)
    {
        auto data = std::dynamic_pointer_cast<InGameContextData>(_data);
        Player& currentPlayer = _players[data->currentPlayerId];

        auto x_max = _data->screenWidth - lr_border - currentPlayer.paddle()->width / 2;
        auto x_min = lr_border + currentPlayer.paddle()->width / 2;
        if (currentPlayer.paddle()->y >= gap_width + currentPlayer.paddle()->height / 2
                && currentPlayer.paddle()->y <= (_data->screenHeight + gap_width)/2 - currentPlayer.paddle()->height / 2)
        {
            switch (currentPlayer.getSide())
            {
            case BoardSide::LEFT:
                x_min = currentPlayer.paddle()->width / 2;
                break;
            case BoardSide::RIGHT:
                x_max = data->screenWidth - currentPlayer.paddle()->width / 2;
                break;
            }
        }

        auto y_max = _data->screenHeight - tb_border - currentPlayer.paddle()->height / 2;
        auto y_min = tb_border + currentPlayer.paddle()->height  / 2;

        mouse_x = std::min(mouse_x, x_max);
        mouse_x = std::max(mouse_x, x_min);
        mouse_y = std::min(mouse_y, y_max);
        mouse_y = std::max(mouse_y, y_min);

        switch (currentPlayer.getSide())
        {
        case BoardSide::LEFT:
            mouse_x = std::min(mouse_x, data->screenWidth / 2 - currentPlayer.paddle()->width / 2);
            break;
        case BoardSide::RIGHT:
            mouse_x = std::max(mouse_x, data->screenWidth / 2 + currentPlayer.paddle()->width / 2);
            break;
        }
    }

    void InGameContext::checkCollisions()
    {
        //int x = 0;
        //int force = 0;
        //if(Client::getInstance().getCollision(x, force))
        //    playSound(SoundType::COLLISION, x, force);
    }

    void InGameContext::checkGoal()
    {
        auto data = std::dynamic_pointer_cast<InGameContextData>(_data);
        Player& currentPlayer = _players[data->currentPlayerId];

        int playerId = -1;
        int points = -1;
        if (Client::getInstance().getGoal(playerId, points))
        {
            handleGoal(playerId, points);
            Client::getInstance().sendPaddlePos(currentPlayer.paddle()->x, currentPlayer.paddle()->y);

            timers.createUntilTimer(
                TimerFactory::InvokeType::OnRender,
                3,
                true,
                [this](float dt)
                {
                    auto goalFont = _rm->getFont(FontType::SCORE);

                    //TODO: GOAL string should appear on 'goaler' side
                    const char *goalStr = "GOAL";
                    auto strWidth = goalFont->GetStringWidth(goalStr);

                    goalFont->Render(_data->screenWidth/2 - strWidth/2,
                                     _data->screenHeight/2,
                                     HGETEXT_LEFT,
                                     goalStr);
                });
        }

    }

    void InGameContext::handleGoal(int playerId, int points)
    {
        // play a goal sound

        _players[playerId].setPoints(points);
        for (auto& p : _players)
            p.paddle()->resetToInit();
    }



}



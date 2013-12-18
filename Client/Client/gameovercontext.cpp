#include "gameovercontext.h"
#include <hgefont.h>

namespace NeonHockey
{
    GameOverContext::GameOverContext(HGE* hge, std::shared_ptr<ResourceManager> rm, std::shared_ptr<GameOverContextData> data)
        : IContext(hge, rm, data)
    { }

    void GameOverContext::show()
    {

    }

    IContextReturnData GameOverContext::frameFunc()
    {
        auto data = std::dynamic_pointer_cast<GameOverContextData>(_data);
        return IContextReturnData(Context::GameOverContext, data);
    }

    void GameOverContext::renderFunc()
    {
        auto data = std::dynamic_pointer_cast<GameOverContextData>(_data);

        //TODO: написать метод для центрированного вывода текста
        auto fnt = _rm->getFont(FontType::SCORE);

        const char *resultStr = nullptr;

        if(data->_win)
            resultStr = "YOU WON!";
        else
            resultStr = "YOU LOST!";

        int resultStrLen = fnt->GetStringWidth(resultStr);

        fnt->Render(_data->screenWidth/2 - resultStrLen/2, 30,
                    HGETEXT_LEFT, resultStr);

        int screenCenter = data->screenWidth / 2;
        std::string tempStr;

        //render leftPlayer's info
        const char *leftPlayerName = data->_leftPlayer->getName().c_str();
        int leftPlayerNameLen = fnt->GetStringWidth(leftPlayerName);

        tempStr = std::string("Scores: ") + std::to_string(data->_leftPlayer->getPoints());
        const char *leftPlayerScore = tempStr.c_str();
        int leftPlayerScoreLen = fnt->GetStringWidth(tempStr.c_str());

        fnt->Render(screenCenter/2 - leftPlayerNameLen/2, 60,
                    HGETEXT_LEFT, leftPlayerName);

        fnt->Render(screenCenter/2 - leftPlayerScoreLen/2, 80,
                    HGETEXT_LEFT, leftPlayerScore);

        //render rightPlayer's info
        const char *rightPlayerName = data->_rightPlayer->getName().c_str();
        int rightPlayerNameLen = fnt->GetStringWidth(rightPlayerName);

        tempStr = std::string("Scores: ") + std::to_string(data->_rightPlayer->getPoints());
        const char *rightPlayerScore = tempStr.c_str();
        int rightPlayerScoreLen = fnt->GetStringWidth(tempStr.c_str());

        fnt->Render(screenCenter*3/2 - rightPlayerNameLen/2, 60,
                    HGETEXT_LEFT, rightPlayerName);

        fnt->Render(screenCenter*3/2 - rightPlayerScoreLen/2, 80,
                    HGETEXT_LEFT, rightPlayerScore);
    }
}

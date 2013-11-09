#include "game.h"
#include <iostream>

int main()
{
    using namespace NeonHockey;
    try
    {
        Game& game = Game::getInstance();
        game.start();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/*#include "unix_platform.h"
#include "main.h"

#include <vector>
#include <ctime>
#include <cmath>

#include <hge.h>
#include <hgesprite.h>
#include <hgecolor.h>

#include "puck.h"
#include "paddle.h"
#include "resourcemanager.h"


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define MIDDLE_LINE ((SCREEN_HEIGHT) / 2)

using namespace NeonHockey;

HGE* hge = nullptr;
ResourceManager resources;
std::vector<Paddle> paddles;
//paddles.push_back(Paddle(new TextureInfo(0, 0, 64, 64, "resources/zazaka.png")));


bool FrameFunc()
{
    //float dt = hge->Timer_GetDelta();

    return false;
}


bool RenderFunc()
{
    const float screen_width = SCREEN_WIDTH;
    const float screen_height = SCREEN_HEIGHT;
    const float middle_line = MIDDLE_LINE;
    const float paddle_padding = 50.0f;

    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);

    hge->Gfx_RenderLine(0, middle_line, screen_width - 1, middle_line);
    Section top(0, middle_line, screen_width-1, 0);
    Section bottom(0, screen_height-1, screen_width, middle_line);

    auto puckSprite = resources.getSprite(GfxType::PUCK);
    auto paddleSprite = resources.getSprite(GfxType::PADDLE1);

    puckSprite->Render(screen_width/2 - 1, middle_line);
    paddleSprite->Render(bottom.x_rt/2, screen_height - paddle_padding);

    hge->Gfx_EndScene();
    return false;
}

#ifdef PLATFORM_UNIX
int main(int argc, char *argv[])
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    hge = hgeCreate(HGE_VERSION);
    resources.initHge(hge);

    // Set desired system states and initialize HGE
    hge->System_SetState(HGE_LOGFILE, "hge_test.log");
    hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    hge->System_SetState(HGE_TITLE, "HGE Test");
    hge->System_SetState(HGE_USESOUND, false);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_HIDEMOUSE, false);
    hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
    hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
    hge->System_SetState(HGE_SCREENBPP, 32);

    if(hge->System_Initiate())
    {
        // Load textures
        try
        {
            resources.addTexture(GfxType::PUCK, "resources/particles.png");
            resources.addTexture(GfxType::PADDLE1, "resources/zazaka.png");

            resources.addSprite(GfxType::PUCK, 96, 64, 32, 32)->SetHotSpot(16,16);
            resources.addSprite(GfxType::PADDLE1, 0, 0, 64, 64)->SetHotSpot(32,32);


            hge->System_Start();


            resources.freeResources();
            hge->System_Shutdown();
            hge->Release();
        }
        catch(std::exception& e)
        {
#ifdef PLATFORM_UNIX
            fprintf(stderr, "Error: %s\n", e.what());
#else
            MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
#endif
            resources.freeResources();
            hge->System_Shutdown();
            hge->Release();
        }


    }
    return 0;
}
*/

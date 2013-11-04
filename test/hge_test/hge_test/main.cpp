#include "unix_platform.h"

#include <vector>
#include <ctime>
#include <cmath>

#include <hge.h>
#include <hgesprite.h>
#include <hgecolor.h>

#include "puck.h"
#include "resourcemanager.h"


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

using namespace NeonHockey;

HGE* hge = nullptr;
ResourceManager resources;



bool FrameFunc()
{
    float dt = hge->Timer_GetDelta();

    return false;
}


bool RenderFunc()
{
    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);

    auto puckSprite = resources.GetSprite(GfxType::PUCK);
    puckSprite->Render(100.0f, 100.0f);

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
    resources.InitHge(hge);

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
            resources.AddTexture(GfxType::PUCK, "resources/particles.png");
            resources.AddSprite(GfxType::PUCK, 96, 64, 32, 32);

            hge->System_Start();
        }
        catch(std::exception& e)
        {
#ifdef PLATFORM_UNIX
            fprintf(stderr, "Error: ");
            fprintf(stderr, e.what());
            fprintf(stderr, "\n");
#else
            MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
#endif
            resources.FreeResources();
            hge->System_Shutdown();
            hge->Release();
        }
    }


    resources.FreeResources();
    hge->System_Shutdown();
    hge->Release();
    return 0;
}

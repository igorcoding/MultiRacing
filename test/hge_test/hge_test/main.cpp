/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut07 - Thousand of Hares
*/


// Copy the files "font2.fnt", "font2.png", "bg2.png"
// and "zazaka.png" from the folder "precompiled" to
// the folder with executable file. Also copy hge.dll
// to the same folder.



#include <vector>
#include <ctime>
#include <hge.h>
#include <hgesprite.h>
#include <hgecolor.h>



#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

struct Hair
{
    float x;
    float y;
    float dx;
    float dy;
    static float width;
    static float height;
};

float Hair::width = 64;
float Hair::height = 64;

std::vector<Hair> hairs;

// Pointer to the HGE interface (helper classes require this to work)

HGE *hge=0;

// Resource handles

HTEXTURE			tex;
hgeSprite			*spr;


bool FrameFunc()
{
    float dt = hge->Timer_GetDelta();

    const float speed = 50;
    const float friction = 1.0f;

    float& x = hairs[0].x;
    float& y = hairs[0].y;
    float& dx = hairs[0].dx;
    float& dy = hairs[0].dy;

    switch(hge->Input_GetKey())
    {
    case HGEK_UP:		dy -= speed*dt; break;
    case HGEK_DOWN:		dy += speed*dt; break;
    case HGEK_RIGHT:	dx += speed*dt; break;
    case HGEK_LEFT:     dx -= speed*dt; break;
    case HGEK_ESCAPE:   return true;
    }

    dx *= friction;
    dy *= friction;
    x += dx;
    y += dy;

    auto xBorder_max = SCREEN_WIDTH - Hair::width / 2;
    auto xBorder_min = Hair::width / 2;
    auto yBorder_max = SCREEN_HEIGHT - Hair::height / 2;
    auto yBorder_min = Hair::height / 2;

    if (x > xBorder_max) {
        x = xBorder_max - (x - xBorder_max);
        dx = -dx;
    }
    if (x < xBorder_min) {
        x = xBorder_min + xBorder_min - x;
        dx = -dx;
    }
    if (y > yBorder_max) {
        y = yBorder_max - (y - yBorder_max);
        dy = -dy;
    }
    if (y < yBorder_min) {
        y = yBorder_min + yBorder_min - y;
        dy = -dy;
    }


    return false;
}


bool RenderFunc()
{
    hge->Gfx_BeginScene();
    hge->Gfx_Clear(hgeColorRGB(0, 0, 0, 1.0).GetHWColor());
    spr->RenderEx(hairs[0].x, hairs[0].y, 0.0);
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

    // Set desired system states and initialize HGE

    hge->System_SetState(HGE_LOGFILE, "hge_test.log");
    hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    hge->System_SetState(HGE_TITLE, "HGE Test");
    hge->System_SetState(HGE_USESOUND, false);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
    hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
    hge->System_SetState(HGE_SCREENBPP, 32);

    if(hge->System_Initiate())
    {

        // Load textures

        tex = hge->Texture_Load("resources/zazaka.png");

        if(!tex)
        {
            // If one of the data files is not found,
            // display an error message and shutdown
#ifdef PLATFORM_UNIX
            fprintf(stderr, "Error: Can't load zazaka.png\n");
#else
            MessageBox(NULL, "Can't load zazaka.png", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
#endif
            hge->System_Shutdown();
            hge->Release();
            return 0;
        }

        // Load font, create sprites
        spr = new hgeSprite(tex, 0, 0, Hair::width, Hair::height);
        spr->SetHotSpot(Hair::width / 2, Hair::height / 2);

        //bgSprite = new hgeSprite(bgTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        //bgSprite->SetColor(0xFF000000, 0);

        // Init hairs
        hge->Random_Seed(time(0));
        Hair hair;
        hair.x = hge->Random_Float(0,SCREEN_WIDTH);
        hair.y = hge->Random_Float(0,SCREEN_HEIGHT);
        hair.dx = 0.0f;
        hair.dy = 0.0f;
        hairs.emplace_back(hair);

        // Let's rock now!
        hge->System_Start();

        // Delete created objects and free loaded resources

        delete spr;
        hge->Texture_Free(tex);
    }

    // Clean up and shutdown

    hge->System_Shutdown();
    hge->Release();
    return 0;
}

#include "Commoninc.h"
#include "Draw.h"
#include "Video.h"

VideoSystem VideoSys;

int WinMain(int argc, char* argv[]) {
    DrawFunc DrawStuff;

    SDL_Init(SDL_INIT_VIDEO);

    VideoSys.initVideoSystem();
    DrawStuff.drawRectangle();
    VideoSys.deInitVideoSystem();
    SDL_Quit();

    return 0;
}
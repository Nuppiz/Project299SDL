#include "Commoninc.h"
#include "Draw.h"
#include "Video.h"

VideoSystem VideoSys;

int WinMain(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    VideoSys.initVideoSystem();
    DrawFunc::drawRectangle();
    VideoSys.deInitVideoSystem();
    SDL_Quit();

    return 0;
}
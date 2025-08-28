#include "Commoninc.h"
#include "Video.h"

int VideoSystem::initVideoSystem()
{
    window = SDL_CreateWindow("SDL3 Project",640, 480, 0);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
}
void VideoSystem::deInitVideoSystem()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
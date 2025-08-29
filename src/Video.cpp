#include "Commoninc.h"
#include "Video.h"

int VideoSystem::initVideoSystem()
{
    if (!SDL_CreateWindowAndRenderer("Hello SDL", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
}
void VideoSystem::deInitVideoSystem()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
#include "Commoninc.h"
#include "Draw.h"
#include "Video.h"

extern VideoSystem VideoSys;

void DrawFunc::drawRectangle()
{
    SDL_Event e;
    bool quit = false;

    // Define a rectangle
    SDL_FRect greenSquare {270, 190, 100, 100};

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(VideoSys.renderer, 0, 0, 0, 255); // Set render draw color to black
        SDL_RenderClear(VideoSys.renderer); // Clear the renderer

        SDL_SetRenderDrawColor(VideoSys.renderer, 0, 255, 0, 255); // Set render draw color to green
        SDL_RenderFillRect(VideoSys.renderer, &greenSquare); // Render the rectangle
        
        SDL_RenderPresent(VideoSys.renderer); // Render the screen
    }
}
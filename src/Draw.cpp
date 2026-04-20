#include "Commoninc.h"
#include "Draw.h"
#include "Square.h"
#include "Video.h"

extern VideoSystem VideoSys;

KeyBindings wasd { SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D };
KeyBindings arrows { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT };

void DrawFunc::drawRectangle()
{
    SDL_Event e;
    bool quit = false;

    Square greenSquare(270.0f, 190.0f, 100.0f, 200.0f, 0, 255, 0);
    Square redSquare(400.0f, 190.0f, 100.0f, 200.0f, 255, 0, 0);

    Uint64 lastTime = SDL_GetTicks();

    while (!quit) {
        Uint64 now = SDL_GetTicks();
        float deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        int winW, winH;
        SDL_GetWindowSize(VideoSys.window, &winW, &winH);

        greenSquare.update(deltaTime, winW, winH, wasd);
        redSquare.update(deltaTime, winW, winH, arrows);

        greenSquare.resolveCollision(redSquare);

        SDL_SetRenderDrawColor(VideoSys.renderer, 0, 0, 0, 255);
        SDL_RenderClear(VideoSys.renderer);

        greenSquare.draw(VideoSys.renderer);
        redSquare.draw(VideoSys.renderer);

        SDL_RenderPresent(VideoSys.renderer);
    }
}
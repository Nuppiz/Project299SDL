#include "Commoninc.h"
#include "Bullet.h"
#include "Controls.h"
#include "Draw.h"
#include "Square.h"
#include "Tile.h"
#include "Video.h"

VideoSystem VideoSys;

KeyBindings wasd  { SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_D };
KeyBindings empty {};

static Square greenSquare (64.0f,  220.0f, 32.0f, 200.0f, 0,   255, 0);
static Square redSquare   (200.0f, 400.0f, 32.0f, 100.0f, 255, 0,   0,   AIBehavior::PATROL,       200.0f, 400.0f);
static Square blueSquare  (140.0f, 220.0f, 32.0f, 0.0f,   0,   0,   255, AIBehavior::JUMP_IN_PLACE, 0.0f,   0.0f,  0.8f);
static Square yellowSquare(500.0f, 220.0f, 32.0f, 120.0f, 255, 255, 0,   AIBehavior::JUMP_AND_MOVE, 480.0f, 580.0f, 1.2f);

static void gameUpdate(float deltaTime, int winW, int winH)
{
    if (mouseLeftClicked)
        fireBullet(greenSquare.x + greenSquare.size / 2,
                   greenSquare.y + greenSquare.size / 2,
                   mousePos.x, mousePos.y);

    updateBullets(deltaTime);

    greenSquare.update(deltaTime, winW, winH, wasd);
    redSquare.update(deltaTime, winW, winH, empty);
    blueSquare.update(deltaTime, winW, winH, empty);
    yellowSquare.update(deltaTime, winW, winH, empty);

    greenSquare.resolveCollision(redSquare);
    greenSquare.resolveCollision(blueSquare);
    greenSquare.resolveCollision(yellowSquare);
}

static void gameDraw()
{
    SDL_SetRenderDrawColor(VideoSys.renderer, 0, 0, 0, 255);
    SDL_RenderClear(VideoSys.renderer);

    Tile::draw(VideoSys.renderer);
    greenSquare.draw(VideoSys.renderer);
    redSquare.draw(VideoSys.renderer);
    blueSquare.draw(VideoSys.renderer);
    yellowSquare.draw(VideoSys.renderer);
    drawBullets(VideoSys.renderer);
    drawReticle(VideoSys.renderer, mousePos.x, mousePos.y);

    SDL_RenderPresent(VideoSys.renderer);
}

int WinMain(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    VideoSys.initVideoSystem();
    SDL_HideCursor();

    if (!Tile::loadFromFile("level1.txt"))
        return 1;
    if (!Tile::loadTexture(VideoSys.renderer, "Bricks.png"))
        return 1;

    SDL_Event e;
    bool quit = false;
    Uint64 lastTime = SDL_GetTicks();

    while (!quit) {
        Uint64 now = SDL_GetTicks();
        float deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;

        updateControls();

        while (SDL_PollEvent(&e)) {
            if      (e.type == SDL_EVENT_QUIT)                 quit = true;
            else if (e.type == SDL_EVENT_WINDOW_FOCUS_GAINED)  SDL_HideCursor();
            else if (e.type == SDL_EVENT_WINDOW_FOCUS_LOST)    SDL_ShowCursor();
            else handleControlEvent(e);
        }

        int winW, winH;
        SDL_GetWindowSize(VideoSys.window, &winW, &winH);

        gameUpdate(deltaTime, winW, winH);
        gameDraw();
    }

    VideoSys.deInitVideoSystem();
    SDL_Quit();

    return 0;
}

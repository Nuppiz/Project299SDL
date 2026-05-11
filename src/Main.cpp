#include "Commoninc.h"
#include "Actor.h"
#include "Bullet.h"
#include "Controls.h"
#include "Draw.h"
#include "Tile.h"
#include "Video.h"

VideoSystem VideoSys;

static PlayerActor player;

static const int ENEMY_COUNT = 3;
static EnemyActor enemies[ENEMY_COUNT];

static void gameUpdate(float deltaTime, int winW, int winH)
{
    const bool* keys = SDL_GetKeyboardState(nullptr);
    player.control = 0;
    if (keys[SDL_SCANCODE_A]) player.control |= CTRL_LEFT;
    if (keys[SDL_SCANCODE_D]) player.control |= CTRL_RIGHT;
    if (keys[SDL_SCANCODE_W]) player.control |= CTRL_JUMP;

    float centerX    = player.position.x + player.radius_w / 2.0f;
    float centerY    = player.position.y + player.radius_h / 2.0f;
    player.angle     = atan2((double)(mousePos.y - centerY), (double)(mousePos.x - centerX));

    if (mouseLeftClicked)
    {
        fireBullet(centerX, centerY, mousePos.x, mousePos.y);
        player.shootTimer = SHOOT_ANIM_DURATION;
    }

    player.update(deltaTime, winW, winH);
    player.updateAnimation(deltaTime);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        enemies[i].updateAI(deltaTime, player.position);
        enemies[i].update(deltaTime, winW, winH);
        enemies[i].updateAnimation(deltaTime);
    }

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        player.resolveCollision(enemies[i]);
        for (int j = i + 1; j < ENEMY_COUNT; j++)
            enemies[i].resolveCollision(enemies[j]);
    }

    updateBullets(deltaTime);
}

static void gameDraw()
{
    SDL_SetRenderDrawColor(VideoSys.renderer, 0, 0, 0, 255);
    SDL_RenderClear(VideoSys.renderer);

    Tile::draw(VideoSys.renderer);
    player.draw(VideoSys.renderer);
    for (int i = 0; i < ENEMY_COUNT; i++)
        enemies[i].draw(VideoSys.renderer);
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

    player = spawnPlayer(64.0f, 220.0f);
    if (!player.loadSpriteSheet(VideoSys.renderer, "Keen.png"))
        return 1;

    enemies[0] = spawnEnemy(200.0f, 400.0f, 100.0f, AI_PATROL,        200.0f, 400.0f, 1.0f, 200.0f, 80.0f);
    enemies[1] = spawnEnemy(140.0f, 220.0f,   0.0f, AI_JUMP_IN_PLACE, 0.0f,   0.0f,   0.8f);
    enemies[2] = spawnEnemy(500.0f, 220.0f, 120.0f, AI_JUMP_AND_MOVE, 480.0f, 580.0f, 1.2f, 250.0f, 80.0f);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        if (!enemies[i].loadSpriteSheet(VideoSys.renderer, "Rigelan.png"))
            return 1;
    }

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

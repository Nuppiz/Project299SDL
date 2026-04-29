#include "Commoninc.h"
#include "Bullet.h"
#include "Draw.h"

void drawBullets(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active) continue;
        SDL_FRect rect { bullets[i].x, bullets[i].y, BULLET_SIZE, BULLET_SIZE };
        SDL_RenderFillRect(renderer, &rect);
    }
}

void drawReticle(SDL_Renderer* renderer, float x, float y)
{
    const float GAP = 4.0f;
    const float LEN = 8.0f;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, x - GAP - LEN, y,             x - GAP,       y            );
    SDL_RenderLine(renderer, x + GAP,       y,             x + GAP + LEN, y            );
    SDL_RenderLine(renderer, x,             y - GAP - LEN, x,             y - GAP      );
    SDL_RenderLine(renderer, x,             y + GAP,       x,             y + GAP + LEN);
}
#include "Square.h"

Square::Square(float startX, float startY, float size, float speed, uint8_t colorR, uint8_t colorG, uint8_t colorB)
    : x(startX), y(startY), size(size), speed(speed), colorR(colorR), colorG(colorG), colorB(colorB)
{
}

void Square::update(float deltaTime, int winW, int winH, const KeyBindings& bindings)
{
    const bool* keys = SDL_GetKeyboardState(nullptr);

    if (keys[bindings.up])    y -= speed * deltaTime;
    if (keys[bindings.down])  y += speed * deltaTime;
    if (keys[bindings.left])  x -= speed * deltaTime;
    if (keys[bindings.right]) x += speed * deltaTime;

    x = SDL_clamp(x, 0.0f, (float)winW - size);
    y = SDL_clamp(y, 0.0f, (float)winH - size);
}

void Square::draw(SDL_Renderer* renderer)
{
    SDL_FRect rect { x, y, size, size };
    SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Square::resolveCollision(Square& other)
{
    // Calculate overlap on each axis
    float overlapX = SDL_min(x + size, other.x + other.size) - SDL_max(x, other.x);
    float overlapY = SDL_min(y + size, other.y + other.size) - SDL_max(y, other.y);

    // No collision if either axis has no overlap
    if (overlapX <= 0 || overlapY <= 0) return;

    // Push apart along the axis with the smallest overlap
    float push = 0.0f;
    if (overlapX < overlapY)
    {
        push = overlapX / 2.0f;
        if (x < other.x) { x -= push; other.x += push; }
        else             { x += push; other.x -= push; }
    }
    else
    {
        push = overlapY / 2.0f;
        if (y < other.y) { y -= push; other.y += push; }
        else             { y += push; other.y -= push; }
    }
}
#include "Square.h"
#include "Tile.h"

Square::Square(float startX, float startY, float size, float speed, Uint8 r, Uint8 g, Uint8 b,
               AIBehavior aiBehavior,
               float patrolLeft, float patrolRight,
               float jumpInterval)
    : x(startX), y(startY), size(size), speed(speed),
      vy(0.0f), onGround(false), r(r), g(g), b(b),
      aiBehavior(aiBehavior),
      patrolLeft(patrolLeft), patrolRight(patrolRight),
      aiMoveDir(1.0f),
      jumpTimer(0.0f), jumpInterval(jumpInterval)
{
}

void Square::updateAI(float deltaTime)
{
    const float JUMP_FORCE = 400.0f;

    switch (aiBehavior)
    {
        case AIBehavior::PATROL:
        {
            x += speed * aiMoveDir * deltaTime;
            if (x >= patrolRight) { x = patrolRight; aiMoveDir = -1.0f; }
            if (x <= patrolLeft)  { x = patrolLeft;  aiMoveDir =  1.0f; }
            break;
        }
        case AIBehavior::JUMP_IN_PLACE:
        {
            jumpTimer -= deltaTime;
            if (jumpTimer <= 0.0f && onGround)
            {
                vy = -JUMP_FORCE;
                onGround = false;
                jumpTimer = jumpInterval;
            }
            break;
        }
        case AIBehavior::JUMP_AND_MOVE:
        {
            // Patrol left/right
            x += speed * aiMoveDir * deltaTime;
            if (x >= patrolRight) { x = patrolRight; aiMoveDir = -1.0f; }
            if (x <= patrolLeft)  { x = patrolLeft;  aiMoveDir =  1.0f; }

            // Jump periodically
            jumpTimer -= deltaTime;
            if (jumpTimer <= 0.0f && onGround)
            {
                vy = -JUMP_FORCE;
                onGround = false;
                jumpTimer = jumpInterval;
            }
            break;
        }
        default:
            break;
    }
}

void Square::update(float deltaTime, int winW, int winH, const KeyBindings& bindings)
{
    const float GRAVITY = 800.0f;

    if (aiBehavior == AIBehavior::NONE)
    {
        // Player controlled
        const bool* keys = SDL_GetKeyboardState(nullptr);

        float newX = x;
        if (keys[bindings.left])  newX -= speed * deltaTime;
        if (keys[bindings.right]) newX += speed * deltaTime;

        if (!Tile::collidesWithWalls(newX, y, size))
            x = newX;

        if (keys[bindings.jump] && onGround)
        {
            vy = -400.0f;
            onGround = false;
        }
    }
    else
    {
        updateAI(deltaTime);
    }

    // Gravity and vertical collision (applies to all squares)
    vy += GRAVITY * deltaTime;

    float newY = y + vy * deltaTime;
    if (!Tile::collidesWithWalls(x, newY, size))
    {
        y = newY;
        onGround = false;
    }
    else
    {
        if (vy > 0) onGround = true;
        vy = 0.0f;
    }

    x = SDL_clamp(x, 0.0f, (float)winW - size);
    y = SDL_clamp(y, 0.0f, (float)winH - size);
}

void Square::draw(SDL_Renderer* renderer)
{
    SDL_FRect rect { x, y, size, size };
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Square::resolveCollision(Square& other)
{
    float overlapX = SDL_min(x + size, other.x + other.size) - SDL_max(x, other.x);
    float overlapY = SDL_min(y + size, other.y + other.size) - SDL_max(y, other.y);

    if (overlapX <= 0 || overlapY <= 0) return;

    if (overlapX < overlapY)
    {
        float push     = overlapX / 2.0f;
        float thisDir  = (x < other.x) ? -1.0f : 1.0f;
        float otherDir = -thisDir;

        bool thisCanMove  = !Tile::collidesWithWalls(x       + thisDir  * push, y,       size);
        bool otherCanMove = !Tile::collidesWithWalls(other.x + otherDir * push, other.y, other.size);

        if      (thisCanMove && otherCanMove) { x += thisDir * push;       other.x += otherDir * push; }
        else if (thisCanMove)                 { x += thisDir * overlapX; }
        else if (otherCanMove)                { other.x += otherDir * overlapX; }
    }
    else
    {
        float push     = overlapY / 2.0f;
        float thisDir  = (y < other.y) ? -1.0f : 1.0f;
        float otherDir = -thisDir;

        bool thisCanMove  = !Tile::collidesWithWalls(x,       y       + thisDir  * push, size);
        bool otherCanMove = !Tile::collidesWithWalls(other.x, other.y + otherDir * push, other.size);

        if      (thisCanMove && otherCanMove) { y += thisDir * push;       other.y += otherDir * push; }
        else if (thisCanMove)                 { y += thisDir * overlapY; }
        else if (otherCanMove)                { other.y += otherDir * overlapY; }
    }
}
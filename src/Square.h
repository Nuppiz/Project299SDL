#ifndef SQUARE_H
#define SQUARE_H

// this is just for early learning purposes only

#include "Commoninc.h"

struct KeyBindings
{
    SDL_Scancode jump, left, right;
};

enum class AIBehavior
{
    NONE,
    PATROL,        // Moves left/right between two points
    JUMP_IN_PLACE, // Jumps repeatedly without moving
    JUMP_AND_MOVE  // Jumps and patrols at the same time
};

class Square
{
public:
    float x, y;
    float size;
    float speed;
    float vy;          // Vertical velocity
    bool onGround;     // Whether the square is standing on something
    Uint8 r, g, b;

    AIBehavior aiBehavior;
    float patrolLeft;   // Left boundary for patrol
    float patrolRight;  // Right boundary for patrol
    float aiMoveDir;    // 1.0f = right, -1.0f = left
    float jumpTimer;    // Time until next jump
    float jumpInterval; // How often to jump (seconds)

    Square(float startX, float startY, float size, float speed, Uint8 r, Uint8 g, Uint8 b,
           AIBehavior aiBehavior = AIBehavior::NONE,
           float patrolLeft = 0.0f, float patrolRight = 0.0f,
           float jumpInterval = 1.0f);

    void update(float deltaTime, int winW, int winH, const KeyBindings& bindings);
    void updateAI(float deltaTime);
    void draw(SDL_Renderer* renderer);
    void resolveCollision(Square& other);
};

#endif /* SQUARE_H */

#ifndef SQUARE_H
#define SQUARE_H

// this is just for early learning purposes only

#include "Commoninc.h"

struct KeyBindings
{
    SDL_Scancode up, down, left, right;
};

class Square
{
public:
    float x, y;
    float size;
    float speed;
    uint8_t colorR, colorG, colorB;

    Square(float startX, float startY, float size, float speed, uint8_t colorR, uint8_t colorG, uint8_t colorB);

    void update(float deltaTime, int winW, int winH, const KeyBindings& keys);
    void draw(SDL_Renderer* renderer);
    void resolveCollision(Square& other);
};

#endif /* SQUARE_H */

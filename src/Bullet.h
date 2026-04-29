#ifndef BULLET_H
#define BULLET_H

const int   MAX_BULLETS  = 64;
const float BULLET_SPEED = 500.0f;
const float BULLET_SIZE  = 4.0f;

struct Bullet
{
    float x, y;
    float vx, vy;
    bool  active;
};

extern Bullet bullets[MAX_BULLETS];

void fireBullet(float originX, float originY, float targetX, float targetY);
void updateBullets(float deltaTime);

#endif /* BULLET_H */

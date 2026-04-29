#include <cmath>
#include "Bullet.h"
#include "Tile.h"

Bullet bullets[MAX_BULLETS] = {};

void fireBullet(float originX, float originY, float targetX, float targetY)
{
    float dx  = targetX - originX;
    float dy  = targetY - originY;
    float len = sqrtf(dx * dx + dy * dy);
    if (len == 0.0f) return;

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active) continue;
        bullets[i].x      = originX;
        bullets[i].y      = originY;
        bullets[i].vx     = (dx / len) * BULLET_SPEED;
        bullets[i].vy     = (dy / len) * BULLET_SPEED;
        bullets[i].active = true;
        break;
    }
}

void updateBullets(float deltaTime)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active) continue;
        bullets[i].x += bullets[i].vx * deltaTime;
        bullets[i].y += bullets[i].vy * deltaTime;
        if (Tile::collidesWithWalls(bullets[i].x, bullets[i].y, BULLET_SIZE))
            bullets[i].active = false;
    }
}

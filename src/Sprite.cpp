#include "Sprite.h"

// { frameCount, frameDuration, sheetRow }
AnimDef animDefs[ANIM_COUNT] =
{
    { 1, 0.01f,  0 }, // ANIM_STAND_LEFT
    { 1, 0.01f,  1 }, // ANIM_STAND_RIGHT
    { 4, 0.12f,  2 }, // ANIM_WALK_LEFT
    { 4, 0.12f,  3 }, // ANIM_WALK_RIGHT
    { 3, 0.20f,  4 }, // ANIM_JUMP_LEFT
    { 3, 0.20f,  5 }, // ANIM_JUMP_RIGHT
    { 1, 0.08f,  6 }, // ANIM_SHOOT_LEFT_STAND
    { 1, 0.08f,  7 }, // ANIM_SHOOT_LEFT_JUMP
    { 1, 0.08f,  8 }, // ANIM_SHOOT_DOWN
    { 1, 0.08f,  9 }, // ANIM_SHOOT_UP_JUMP
    { 1, 0.08f, 10 }, // ANIM_SHOOT_UP_STAND
    { 1, 0.08f, 11 }, // ANIM_SHOOT_RIGHT_STAND
    { 1, 0.08f, 12 }, // ANIM_SHOOT_RIGHT_JUMP
};

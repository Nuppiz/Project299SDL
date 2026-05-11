#ifndef SPRITE_H
#define SPRITE_H

const int   ANIM_FRAME_W    = 32;
const int   ANIM_FRAME_H    = 40;
const int   ANIM_COUNT      = 13;
const int   ANIM_MAX_FRAMES = 4;
const float SHOOT_ANIM_DURATION = 0.15f;

enum AnimState
{
    ANIM_STAND_LEFT = 0,
    ANIM_STAND_RIGHT,
    ANIM_WALK_LEFT,
    ANIM_WALK_RIGHT,
    ANIM_JUMP_LEFT,
    ANIM_JUMP_RIGHT,
    ANIM_SHOOT_LEFT_STAND,
    ANIM_SHOOT_LEFT_JUMP,
    ANIM_SHOOT_DOWN,
    ANIM_SHOOT_UP_JUMP,
    ANIM_SHOOT_UP_STAND,
    ANIM_SHOOT_RIGHT_STAND,
    ANIM_SHOOT_RIGHT_JUMP
};

struct AnimDef
{
    int   frameCount;    // number of frames (1–4)
    float frameDuration; // seconds per frame
    int   sheetRow;      // row in the sprite sheet
};

extern AnimDef animDefs[ANIM_COUNT];

#endif /* SPRITE_H */

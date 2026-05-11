#ifndef ACTOR_H
#define ACTOR_H

#include "Commondef.h"
#include "Commonstruct.h"
#include "Sprite.h"

// Control bitmask flags
const flags_t CTRL_LEFT  = (1 << 0);
const flags_t CTRL_RIGHT = (1 << 1);
const flags_t CTRL_JUMP  = (1 << 2);

// AI mode values for EnemyActor::ai_mode
const uint8_t AI_NONE          = 0;
const uint8_t AI_PATROL        = 1;
const uint8_t AI_JUMP_IN_PLACE = 2;
const uint8_t AI_JUMP_AND_MOVE = 3;
const uint8_t AI_CHASE         = 4;

class Actor
{
public:
    id_t     id;             // id in the object array
    flags_t  flags;          // special actor options
    Vec2     position;       // exact location on the map
    Vec2_int grid_loc;       // location on the grid
    Vec2     direction;      // direction
    double   angle;          // current direction in radians
    Vec2     velocity;       // velocity (x=horizontal, y=vertical)
    float    magnitude;      // magnitude of velocity
    float    move_speed;     // movement speed in pixels/sec
    uint8_t  collision_type; // 1=circle 2=box
    int      radius;         // collision circle radius
    int      radius_w;       // collision box width
    int      radius_h;       // collision box height
    int      health;         // self-explanatory
    bool     onGround;       // true when standing on solid ground

    Uint8    r, g, b;        // fallback draw color when no sprite sheet is loaded

    flags_t  control;        // which control "buttons" are being pressed
    ticks_t  last_shot;      // last shot taken
    int      weapon_id_1;    // primary weapon
    int      weapon_id_2;    // secondary weapon (melee for enemies)

    SDL_Texture* spriteSheet;
    AnimState    animState;
    int          animFrame;
    float        animTimer;
    float        shootTimer; // counts down after firing; >0 triggers shoot animation

    bool      loadSpriteSheet(SDL_Renderer* renderer, const std::string& filename);
    AnimState selectAnimation() const;
    void      updateAnimation(float deltaTime);
    void      update(float deltaTime, int winW, int winH);
    void      resolveCollision(Actor& other);
    void      draw(SDL_Renderer* renderer);
};

class EnemyActor : public Actor
{
public:
    uint8_t ai_mode;
    ticks_t ai_timer;             // delay between switching to another action
    id_t    target_id_primary;
    id_t    target_id_secondary;
    Vec2    move_target;          // target position for movement AI
    id_t    trigger_on_death;

    float   patrolLeft;
    float   patrolRight;
    float   aiMoveDir;            // 1.0=right, -1.0=left
    float   jumpTimer;
    float   jumpInterval;
    float   sight_range;          // distance at which chase activates
    float   attack_range;         // distance at which chasing stops

    void updateAI(float deltaTime, const Vec2& playerPos);

    int  testLineOfSight(Vec2 p, Vec2 target);
    int  testFieldOfView(Vec2 origin, Vec2 direction, Vec2 target);
    void AIChaseTarget(EnemyActor* chaser);
    void AIThink(EnemyActor* actor);
    void AIAct(EnemyActor* actor);
};

class PlayerActor : public Actor
{
public:
    // additional weapon slots
    int weapon_id_3;
    int weapon_id_4;
    int weapon_id_5;
    int weapon_id_6;

    // encumbrance
    int current_encumbrance;
    int max_encumbrance;

    // stats
    int strength;
    int endurance;
    int speed;
    int agility;
};

PlayerActor spawnPlayer(float x, float y);
EnemyActor  spawnEnemy(float x, float y, float speed, uint8_t aiMode = AI_NONE,
                       float patrolLeft = 0.0f, float patrolRight = 0.0f,
                       float jumpInterval = 1.0f,
                       float sightRange = 0.0f, float attackRange = 0.0f);

#endif /* ACTOR_H */

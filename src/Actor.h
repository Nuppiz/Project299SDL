#ifndef ACTOR_H
#define ACTOR_H

#include "Commondef.h"
#include "Commonstruct.h"

class Actor
{
public:
    id_t     id;             // id in the object array
    flags_t  flags;          // special actor options
    Vec2     position;       // exact location on the map
    Vec2_int grid_loc;       // location on the grid
    Vec2     direction;      // direction
    double   angle;          // current direction in radians
    Vec2     velocity;       // velocity
    float    magnitude;      // magnitude of velocity
    uint8_t  collision_type; // 1=circle 2=box
    int      radius;         // collision circle radius
    int      radius_w;       // collision box width
    int      radius_h;       // collision box height
    int      health;         // self-explanatory
    
    flags_t control; // which control "buttons" are being pressed

    id_t texture_id; // replace with sprite/animation thing
    ticks_t last_shot; // last shot taken
    int weapon_id_1; // primary weapon
    int weapon_id_2; // secondary weapon (melee for enemies)
};

class EnemyActor : public Actor
{
    uint8_t ai_mode; // type of action the AI is taking
    ticks_t ai_timer; // delay between switching to another action
    id_t    target_id_primary; // index number of the AI's primary target in the ActorsById array
    id_t    target_id_secondary; // index number of the AI's secondary target in the ActorsById array
    Vec2    move_target; // target position for character movement
    id_t trigger_on_death; // entity ID to trigger on death

    int testLineOfSight(Vec2 p, Vec2 target);
    int testFieldOfView(Vec2 origin, Vec2 direction, Vec2 target);
    void AIChaseTarget(EnemyActor* chaser);
    void AIThink(EnemyActor* actor);
    void AIAct(EnemyActor* actor);
};

class PlayerActor : public Actor
{
    // additional weaon slots
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

#endif /* ACTOR_H */

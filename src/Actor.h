#ifndef ACTOR_H
#define ACTOR_H

#include "Commondef.h"
#include "Commonstruct.h"

class Actor
{
private:
    id_t     id;
    flags_t  flags;
    Vec2     position;
    Vec2_int grid_loc;  // location on the grid
    Vec2     direction; // direction
    double   angle;     // current direction in radians
    Vec2     velocity;  // velocity
    float    magnitude; // magnitude of velocity
    int      radius;    // collision box/circle radius
    int      health;
    
    flags_t control;
    uint8_t ai_mode;
    ticks_t ai_timer;
    id_t    target_id_primary; // index number of the AI's primary target in the ActorsById array
    id_t    target_id_secondary; // index number of the AI's secondary target in the ActorsById array
    Vec2    move_target;

    id_t texture_id;
    id_t trigger_on_death; // entity ID to trigger on death
    ticks_t last_shot; // last shot taken
    int primary_weapon_id;
    int secondary_weapon_id;
};

#endif /* ACTOR_H */

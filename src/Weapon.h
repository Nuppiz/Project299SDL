#ifndef WEAPON_H
#define WEAPON_H

#include "Commondef.h"
#include "Commonstruct.h"

class Weapon
{
    id_t id; // id number in the weapons array
    char name[20];
    uint16_t range; // absolute maximum range for projectiles/hitting the target (if melee)
    uint16_t damage; // base damage per projectile/hit/etc.
    uint8_t num_projectiles; // number of projectiles per shot (e.g. a shotgun fires multiple pellets)
    int8_t projectile_speed; // if -1, instant (hitscan)
    uint8_t projectile_spread; // maximum of how much each projectile diverts from the targeted point (i.e. accuracy)
    ticks_t shot_delay; // delay between each shot/swing/whatever
    int8_t ammo_type; // if -1, infinite ammo/melee
    uint8_t sound_id; // id number of weapon firing sound
    int16_t weapon_sprite_id; // id number for weapon sprite
    int16_t projectile_sprite_id; // id number for projectile sprite (if not hitscan), -1 for none
    int8_t effect_id; // effect to spawn on hit (explosion, poison etc.), -1 for no effect
};

class WeaponEffect
{
    uint8_t sound_id; // id number to the enum table that contains sound effects
    uint16_t damage; // damage at the center of the effect
    uint16_t radius; // radius for explosion / poison splash etc.
    id_t sprite_id; // animation graphic
};

class Projectile
{
    id_t source_id; // id number of the shooter (for infighting etc.)
    Vec2 origin;
    Vec2 position;
    Vec2 velocity;
    double angle;
    Vec2_int grid_loc;
    uint16_t max_range;
    uint16_t damage;
    int sprite_id; // id in the TempSprites array
    int8_t effect_id; // effect to spawn on "death"
    uint8_t state;
};

#endif /* WEAPON_H */

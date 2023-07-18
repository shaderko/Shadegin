/**
 * @file ship.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SHIP_H
#define SHIP_H

#include <linmath.h>

#include "../../src/engine/object/game_object/game_object.h"

typedef struct Ship Ship;
struct Ship
{
    int id;
    int owner_id;
    int type;
    int faction;
    int level;
    int rank;
    int experience;

    GameObject *object;
    // Canon[] canons

    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    float max_acceleration;
    float rotation;

    float mass;
    float max_force;
    float max_turn_rate;
    float max_health;
    float health;
    float max_shield;
    float shield;
    float max_shield_regen;
    float shield_regen;
    float max_energy;
    float energy;
    float max_energy_regen;
    float energy_regen;
    float cargo_capacity;
    float cargo;
};

struct AShip
{
    Ship *(*Init)(Model *model);
    void (*Update)(Ship *ship);
    void (*Destroy)(Ship *ship);
};

extern struct AShip AShip[1];

#endif
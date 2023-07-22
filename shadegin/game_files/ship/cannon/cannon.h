/**
 * @file cannon.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CANNON
#define CANNON

#include <linmath.h>
#include <stdbool.h>

#include "../../../src/engine/object/object.h"
#include "../../../src/engine/object/model/model.h"

typedef struct Cannon Cannon;
struct Cannon
{
    Object *object;

    bool reloding;
    bool loaded;

    float ammo;
    float max_ammo;
    float heat;
    float max_heat;
};

typedef struct ShipCannon ShipCannon;
struct ShipCannon
{
    vec3 position;
    vec3 rotation;

    Cannon *cannon;
};

struct ACannon
{
    Cannon *(*Init)(Model *model, float max_ammo, float max_heat);

    void (*Render)(Cannon *cannon, vec3 position);

    void (*Destroy)(Cannon *cannon);
};

extern struct ACannon ACannon;

#endif
/**
 * @file wheel.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WHEEL
#define WHEEL

#include <linmath.h>
#include <stdbool.h>

#include "../../../src/engine/object/object.h"
#include "../../../src/engine/object/model/model.h"

typedef struct Wheel Wheel;
struct Wheel
{
    Object *object;

    bool reloding;
    bool loaded;

    float ammo;
    float max_ammo;
    float heat;
    float max_heat;
};

typedef struct ShipWheel ShipWheel;
struct ShipWheel
{
    vec3 position;
    vec3 rotation;

    Wheel *wheel;
};

struct AWheel
{
    Wheel *(*Init)(Model *model, float max_ammo, float max_heat);

    void (*Render)(Wheel *wheel);

    void (*Destroy)(Wheel *wheel);
};

extern struct AWheel AWheel;

#endif
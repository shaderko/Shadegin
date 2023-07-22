/**
 * @file wheel.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "wheel.h"

static Wheel *Init(Model *model, float max_ammo, float max_heat)
{
    Wheel *wheel = malloc(sizeof(Wheel));
    if (!wheel)
        ERROR_EXIT("Couldn't allocate memory for Wheel");

    wheel->object = AObject.InitMesh(true, false, 0, (vec3){0, 0, 0}, (vec3){1, 1, 1}, model);
    wheel->max_ammo = max_ammo;
    wheel->max_heat = max_heat;

    return wheel;
}

static void Render(Wheel *wheel)
{
    AObject.Render(wheel->object);
}

static void Destroy(Wheel *wheel)
{
    // TODO:
}

struct AWheel AWheel = {
    .Init = Init,
    .Render = Render,
    .Destroy = Destroy,
};
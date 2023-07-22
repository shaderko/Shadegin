/**
 * @file cannon.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cannon.h"

static Cannon *Init(Model *model, float max_ammo, float max_heat)
{
    Cannon *cannon = malloc(sizeof(Cannon));
    if (!cannon)
        ERROR_EXIT("Couldn't allocate memory for Cannon");

    cannon->object = AObject.InitMesh(true, 0, (vec3){0, 0, 0}, (vec3){20, 20, 20}, model);
    cannon->max_ammo = max_ammo;
    cannon->max_heat = max_heat;

    return cannon;
}

static void Render(Cannon *cannon)
{
    AObject.Render(cannon->object);
}

static void Destroy(Cannon *cannon)
{
    // TODO:
}

struct ACannon ACannon = {
    .Init = Init,
    .Render = Render,
    .Destroy = Destroy,
};
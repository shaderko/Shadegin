/**
 * @file ship.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "ship.h"

static Ship *Init(Model *model)
{
    Ship *ship = calloc(1, sizeof(Ship));

    ship->object = AGameObject->InitMesh(false, 10, (vec3){0, 0, 0}, (vec3){10, 10, 10}, model);

    return ship;
}

static void Update(Ship *ship)
{
}

static void Destroy(Ship *ship)
{
    free(ship);
}

struct AShip AShip[1] = {{
    Init,
    Update,
    Destroy,
}};
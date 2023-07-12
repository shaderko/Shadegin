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

static Ship Init()
{
    Ship ship = {0};

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
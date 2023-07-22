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
#include "../../src/engine/object/object.h"

static Ship **ships = NULL;
static int ship_count = 0;

static Ship *Init(Model *model)
{
    Ship *ship = calloc(1, sizeof(Ship));

    ship->object = AObject.InitMesh(true, false, 10, (vec3){0, 0, 0}, (vec3){10, 10, 10}, model);

    // wheel
    ship->wheel = malloc(sizeof(ShipWheel));
    memcpy(ship->wheel->position, (vec3){0, 0, 0}, sizeof(vec3));

    // cannons
    ship->canons = malloc(sizeof(ShipWheel *) * ship->cannon_size);
    for (int i = 0; i < ship->max_cannons; i++)
    {
        ship->canons[i] = malloc(sizeof(ShipCannon));
        memcpy(ship->canons[i]->position, (vec3){100, 0, 0}, sizeof(vec3));
    }

    ships = realloc(ships, sizeof(Ship *) * (ship_count + 1));
    ships[ship_count] = ship;
    ship_count++;

    return ship;
}

static void Move(Ship *ship, vec3 position)
{
}

static void Render(Ship *ship)
{
    // render ship
    AObject.Render(ship->object);

    // render cannons
    for (int i = 0; i < ship->max_cannons; i++)
    {
        if (ship->canons[i]->cannon)
        {
            printf("Rendering cannon\n");
            ACannon.Render(ship->canons[i]->cannon, ship->canons[i]->position);
        }
    }

    // render wheel
    if (ship->wheel->wheel)
    {
        AWheel.Render(ship->wheel->wheel);
    }
}

static void RenderShips()
{
    for (int i = 0; i < 10; i++)
    {
        Render(ships[i]);
    }
}

static void AddWheel(Ship *ship, Wheel *wheel)
{
    ship->wheel->wheel = wheel;
}

static void AddCannon(Ship *ship, Cannon *cannon)
{
    if (ship->cannon_size >= ship->max_cannons)
    {
        printf("Couldn't add a cannon because all the cannons are already occupied");
        return;
    }

    ship->canons[ship->cannon_size]->cannon = cannon;
    ship->cannon_size++;

    return;
}

static void Update(Ship *ship)
{
    // vec3 position;

    // ship->object->velocity[0] =

    // AObject.UpdatePosition(ship->object, position);
}

static void Destroy(Ship *ship)
{
    // free cannons
    for (int i = 0; i < ship->max_cannons; i++)
    {
        if (ship->canons[i]->cannon)
        {
            ACannon.Destroy(ship->canons[i]->cannon);
        }
        free(ship->canons[i]);
    }
    free(ship->canons);

    // free wheel
    if (ship->wheel->wheel)
    {
        AWheel.Destroy(ship->wheel->wheel);
    }

    free(ship);

    printf("Ship destroyed\n");
}

struct AShip AShip[1] = {{
    Init,
    Render,
    RenderShips,
    Update,
    AddCannon,
    AddWheel,
    Destroy,
}};
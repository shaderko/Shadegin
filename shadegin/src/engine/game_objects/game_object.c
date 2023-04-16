/**
 * @file game_object.c
 * @author Filip Lukovic (filiplukovic0@gmail.com)
 * @brief Game object is used to simulate physics and collisions
 * @version 0.1
 * @date 2023-04-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "game_object.h"

static GameObject *Create()
{
    GameObject *object = malloc(sizeof(GameObject));

    object->position_x = 10;

    return object;
}

static void Update(GameObject *object)
{
    object->position_x += 1;
}

struct AGameObject AGameObject[1] =
    {{Create,
      Update}};
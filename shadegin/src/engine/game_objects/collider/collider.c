/**
 * @file collider.c
 * @author https://github.com/shaderko
 * @brief Collider used for collisions
 * @version 0.1
 * @date 2023-04-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "collider.h"
#include "../../util.h"
#include "box_collider.h"

static void Delete(Collider *collider)
{
    free(collider);
}

static Collider *Init()
{
    Collider *collider = malloc(sizeof(Collider));
    if (!collider)
    {
        ERROR_EXIT("error allocating memory for collider.");
    }

    collider->Delete = Delete;

    return collider;
}

static Collider *InitBox(vec3 position, vec3 size)
{
    Collider *collider = malloc(sizeof(Collider));
    if (!collider)
    {
        ERROR_EXIT("error allocating memory for collider.");
    }

    memcpy(collider->position, position, sizeof(vec3));
    ABoxCollider->Init(collider, size);

    return collider;
}

struct ACollider ACollider[1] =
    {{Init,
      InitBox}};
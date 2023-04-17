/**
 * @file box_collider.c
 * @author https://github.com/shaderko
 * @brief Box collider used by Collider class
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdlib.h>
#include "box_collider.h"

static bool Collide(Collider *collider)
{
    return false;
}

static void Delete(Collider *collider)
{
    BoxCollider *child = collider->derived;

    free(child);
    free(collider);
}

Collider *Init(float width, float height, float depth)
{
    BoxCollider *box_collider;
    Collider *collider = create_collider();

    box_collider = malloc(sizeof(BoxCollider));
    if (box_collider == NULL)
    {
        collider->Delete(collider);
        return NULL; // TODO: error exit
    }
    collider->derived = box_collider;
    box_collider->parent = collider;

    box_collider->width = width;
    box_collider->height = height;
    box_collider->depth = depth;

    collider->Collide = Collide;
    collider->Delete = Delete;

    return collider;
}

struct ABoxCollider ABoxCollider[1] =
    {{Init}};
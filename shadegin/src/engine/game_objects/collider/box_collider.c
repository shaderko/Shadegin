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

#include "../../util.h"
#include <stdlib.h>
#include "box_collider.h"
#include "../game_object.h"

static bool Collide(GameObject *object1, GameObject *object2)
{
    if (object1 == object2)
    {
        return false;
    }
    BoxCollider *square1 = (BoxCollider *)object1->collider->derived;
    BoxCollider *square2 = (BoxCollider *)object2->collider->derived;

    vec3 pos1;
    vec3 pos2;
    vec3_add(pos1, object1->position, object1->collider->position);
    vec3_add(pos2, object2->position, object2->collider->position);

    // Calculate the half-width and half-height for both squares
    float half_width1 = square1->size[0] / 2.0f;
    float half_height1 = square1->size[1] / 2.0f;
    float half_depth1 = square1->size[2] / 2.0f;

    float half_width2 = square2->size[0] / 2.0f;
    float half_height2 = square2->size[1] / 2.0f;
    float half_depth2 = square2->size[2] / 2.0f;

    // Check if the squares are overlapping along the x-axis
    bool overlap_x = fabs(pos1[0] - pos2[0]) < (half_width1 + half_width2);

    // Check if the squares are overlapping along the y-axis
    bool overlap_y = fabs(pos1[1] - pos2[1]) < (half_height1 + half_height2);

    bool overlap_z = fabs(pos1[2] - pos2[2]) < (half_depth1 + half_depth2);

    // If the squares are overlapping along both the x and y axes, they are colliding
    return overlap_x && overlap_y && overlap_z;
}

static void Delete(Collider *collider)
{
    BoxCollider *child = collider->derived;

    free(child);
    free(collider);
}

static vec3 *Size(Collider *collider)
{
    static vec3 defaultSize = {0, 0, 0};
    if (collider == NULL || collider->derived == NULL)
    {
        return &defaultSize;
    };

    BoxCollider *derived_collider = (BoxCollider *)collider->derived;
    return &derived_collider->size;
}

static SerializedDerived Serialize(Collider *collider)
{
    SerializedDerived serialize_collider;
    serialize_collider.len = sizeof(vec3);
    serialize_collider.data = malloc(serialize_collider.len);
    if (!serialize_collider.data)
    {
        ERROR_EXIT("Couldn't allocate memory for serialized collider!\n");
    }
    memcpy(serialize_collider.data, collider->Size(collider), serialize_collider.len);

    return serialize_collider;
}

static Collider *Init(Collider *collider, vec3 size)
{
    BoxCollider *box_collider;
    box_collider = malloc(sizeof(BoxCollider));
    if (box_collider == NULL)
    {
        collider->Delete(collider);
        return NULL; // TODO: error exit
    }
    collider->derived = box_collider;
    collider->type = BOX_COLLIDER;
    box_collider->parent = collider;

    memcpy(box_collider->size, size, sizeof(vec3));

    collider->Collide = Collide;
    collider->Delete = Delete;
    collider->Size = Size;
    collider->Seralize = Serialize;

    return collider;
}

struct ABoxCollider ABoxCollider[1] =
    {{Init}};
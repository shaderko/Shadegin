/**
 * @file box_collider.h
 * @author https://github.com/shaderko
 * @brief Box collider used by Collider class
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "collider.h"
#include <stdbool.h>
#include <linmath.h>

typedef struct BoxCollider BoxCollider;
struct BoxCollider
{
    /**
     * Collider class, parent to Box Collider
     */
    Collider *parent;

    /**
     * Vector 3 size of Box Collider
     */
    vec3 size;
};

struct ABoxCollider
{
    /**
     * Create game object
     *
     * Can have different collider and renderer types
     */
    Collider *(*Init)(Collider *collider, vec3 size);
};

extern struct ABoxCollider ABoxCollider[1];

#endif
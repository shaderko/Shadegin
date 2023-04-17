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

typedef struct BoxCollider BoxCollider;
struct BoxCollider
{
    /**
     * Collider class, parent to Box Collider
     */
    Collider* parent;

    /**
     * Vector 3 size of Box Collider
     */
    float width;
    float height;
    float depth;
};

struct ABoxCollider
{
    /**
     * Create game object
     * 
     * Can have different collider and renderer types
     */
    Collider* (*Init)   (float width, float height, float depth);
};

extern struct ABoxCollider ABoxCollider[1];

#endif
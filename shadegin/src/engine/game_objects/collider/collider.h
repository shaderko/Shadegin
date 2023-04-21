/**
 * @file collider.h
 * @author https://github.com/shaderko
 * @brief Collisions used for physics
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef COLLIDER_H
#define COLLIDER_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

typedef struct Collider Collider;

/**
 * Function pointer definition for different collider types
 */
typedef bool (*ptrCollide)(Collider *);
typedef void (*ptrDelete)(Collider *);

struct Collider
{
    /**
     * Pointer to the subclass
     */
    void *derived;

    ptrCollide Collide;
    ptrDelete Delete;
};

struct ACollider
{
    /**
     * Create game object
     *
     * Can have different collider and renderer types
     */
    Collider *(*Init)();
    Collider *(*InitBox)(float width, float height, float depth);
};

extern struct ACollider ACollider[1];

#endif
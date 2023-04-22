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

#include <linmath.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

typedef struct GameObject GameObject;
typedef struct Collider Collider;

struct Collider
{
    /**
     * Pointer to the subclass
     */
    void *derived;

    /**
     * Local position to game object
     */
    vec3 position;

    bool (*Collide)(GameObject *, GameObject *);
    void (*Delete)(Collider *);
};

struct ACollider
{
    /**
     * TODO:
     */
    Collider *(*Init)();
    Collider *(*InitBox)(vec3 position, vec3 size);
};

extern struct ACollider ACollider[1];

#endif
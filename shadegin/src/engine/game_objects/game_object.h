/**
 * @file game_object.h
 * @author https://github.com/shaderko
 * @brief Object that can have different types of renderer and collider, has collisions and gravity
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "collider/collider.h"
#include "renderer/renderer.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct GameObject GameObject;
struct GameObject
{
    /**
     * Vector 3 position of object in world space
     */
    vec3 position;

    /**
     * Vector 3 velocity in each direction
     */
    vec3 velocity;

    /**
     * Mass represents the speed at which object falls,
     * and how much of velocity is transfered/lost on collision
     */
    float mass;

    /**
     * Is a static or dynamic object (Doesn't move or does)
     */
    bool is_static;

    /**
     * Collider of object
     */
    Collider *collider;

    /**
     * Object renderer
     */
    Renderer *renderer;
};

struct AGameObject
{
    /**
     * Create game object
     *
     * Can have different collider and renderer types
     */
    GameObject *(*Init)();

    /**
     * Create a game object box
     */
    GameObject *(*InitBox)(bool is_static, float mass, vec3 position, vec3 size);

    /**
     * Render game object in position of game object with the local position of renderer
     */
    void (*Render)(GameObject *object);

    /**
     * Updates the position of game object with game objects velocity
     */
    void (*Update)(GameObject *object);
};

extern struct AGameObject AGameObject[1];

#endif
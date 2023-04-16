/**
 * Game Object
 * 
 * Object that can have different types of renderer and collider, has collisions and gravity
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct GameObject GameObject;
struct GameObject
{
    /**
     * Vector 3 position of object in world space
     */
    float position_x;
    float position_y;
    float position_z;

    /**
     * Vector 3 velocity in each direction
     */
    float velocity_x;
    float velocity_y;
    float velocity_z;

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
    // Collider* collider;

    /**
     * Object renderer
     */
    // Renderer* renderer;
};

struct AGameObject
{
    /**
     * Create game object
     * 
     * Can have different collider and renderer types
     */
    GameObject* (*Init)  ();

    /**
     * Updates the position of game object with game objects velocity
     */
    void        (*Update)(GameObject* object);
};

extern struct AGameObject AGameObject[1];

#endif
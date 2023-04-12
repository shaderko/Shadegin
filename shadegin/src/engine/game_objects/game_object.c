// ##Overview
// The game_object.c file provides functions to create, manipulate, and render game objects in a 3D space. The game objects can be any entity in the game world, such as walls, floors, or characters. Each game object can have a collider, and the file includes functions for handling object collisions and gravity simulation.
// 
// ##Data Structures
// GameObject
// The GameObject structure represents a single game object in the world. It contains the following fields:
// 
// position: A vec3 representing the position of the game object in the world.
// size: A vec3 representing the dimensions of the game object.
// velocity: A vec3 representing the velocity of the game object.
// mass: A float representing the mass of the game object.
// is_static: A bool that indicates whether the game object is static (true) or dynamic (false).
// collider: A pointer to a Collider structure that represents the collider associated with the game object.

#include "game_object.h"
#include "../render/render.h"
#include <stdlib.h>

static GameObject** objects = NULL;
static size_t size = 0;


// Creates a new game object with the given parameters and adds it to the internal list of game objects. Returns a pointer to the created game object.
GameObject* add_object(vec3 position, vec3 square_size, vec3 velocity, float mass, bool is_static, ColliderType collider_type, RendererType renderer_type, const void* data) {
    objects = realloc(objects, (size + 1) * sizeof(GameObject*));
    if (!objects) {
        // Handle allocation error TODO:
        return NULL;
    }

    Collider* collider = collider_init(collider_type, data);
    Renderer* renderer = renderer_init(renderer_type, data);

    GameObject* object = calloc(1, sizeof(GameObject));
    memcpy(object->position, position, sizeof(vec3));
    memcpy(object->velocity, velocity, sizeof(vec3));
    object->mass = mass;
    object->is_static = is_static;
    object->collider = collider;
    object->renderer = renderer;
    objects[size] = object;
    size++;

    return object;
}

// Renders all game objects in the world using the render_quad() function from the render module.
void render_game_objects() {
    for (int i = 0; i < size; i++) {
        renderer_render(objects[i]);
    }
}

void render_game_object(GameObject* object) {
    renderer_render(object);
}

// Checks if the given game object is colliding with the ground. Returns true if a collision is detected, and false otherwise.
static bool check_ground_collision(GameObject* object) {
    return (object->position[2] <= (object->position[1] * MIN_DEPTH_VALUE) + GROUND_SIZE && object->position[2] >= (object->position[1] * MIN_DEPTH_VALUE) - GROUND_SIZE);
}

// Simulates the effect of gravity on all dynamic game objects. The gravity parameter represents the force of gravity to apply.
void simulate_gravity(float gravity) {
    for (int i = 0; i < size; i++) {
        GameObject* object = objects[i];
        if (object->is_static) {
            continue;
        }

        float velocity_y = (object->velocity[1] + (gravity * object->mass));

        if (check_ground_collision(object)) {
            object->velocity[1] = 0;
            continue;
        }

        for (int x = 0; x < size; x++) {
            if (x == i) {
                continue;
            }

            if (check_collision(object, objects[x])) {
                // Collision detected TODO:
                velocity_y = 0;
            }
        }

        object->velocity[1] = velocity_y;

        update_position(object);
    }
}

// Updates the position of the given game object based on its current velocity.
void update_position(GameObject* object) {
    object->position[0] += object->velocity[0];
    object->position[1] += object->velocity[1];
}

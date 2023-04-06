#pragma once

#include <linmath.h>
#include "collider.h"

typedef struct gameobject {
    vec3 position;
    vec3 size;
    vec3 velocity;
    float mass;
    bool is_static;
    Collider* collider;
} GameObject;

GameObject* add_object(vec3 position, vec3 square_size, vec3 velocity, float mass, bool is_static, ColliderType collider_type, const void* data);
void render_game_objects();
void simulate_gravity(float gravity);
void update_position(GameObject* object);

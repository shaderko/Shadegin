#pragma once

#include <linmath.h>
#include "collider.h"
#include "renderer.h"

typedef struct gameobject {
    vec3 position;
    vec3 velocity;
    float mass;
    bool is_static;
    Collider* collider;
    Renderer* renderer;
} GameObject;

GameObject* add_object(vec3 position, vec3 square_size, vec3 velocity, float mass, bool is_static, ColliderType collider_type, RendererType renderer_type, const void* data);
void render_game_objects();
void render_game_object(GameObject* object);
void simulate_gravity(float gravity);
void update_position(GameObject* object);

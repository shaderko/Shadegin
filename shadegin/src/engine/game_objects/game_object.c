#include "game_object.h"
#include "../render/render.h"
#include <stdlib.h>

static GameObject* objects = NULL;
static size_t size = 0;

GameObject* add_object(vec3 position, vec3 square_size, vec3 velocity, float mass, bool is_static, ColliderType collider_type, const void* data) {
    objects = realloc(objects, (size + 1) * sizeof(GameObject));
    if (!objects) {
        // Handle allocation error TODO:
        return NULL;
    }

    // SquareCollider data = {0};
    // memcpy(data.size, square_size, sizeof(vec3));
    Collider* collider = collider_init(collider_type, data);

    GameObject object = {0};
    memcpy(object.position, position, sizeof(vec3));
    memcpy(object.size, square_size, sizeof(vec3));
    memcpy(object.velocity, velocity, sizeof(vec3));
    object.mass = mass;
    object.is_static = is_static;
    object.collider = collider;
    objects[size] = object;
    size++;

    return &objects[size];
}

void render_game_objects() {
    for (int i = 0; i < size; i++) {
        render_quad(objects[i].position, objects[i].size, (vec4){.5, 1, 1, 1}, false);
        // vec3 roof_position = {walls[i].position[0], walls[i].position[1] + walls[i].size[1], walls[i].position[2]};
        // vec3 roof_size = {walls[i].size[0], walls[i].size[2], walls[i].size[2]};
        // render_quad(roof_position, roof_size, walls[i].color, false);
    }
}

void simulate_gravity(float gravity) {
    for (int i = 0; i < size; i++) {
        GameObject* object = &objects[i];
        if (object->is_static) {
            return;
        }

        float velocity_y = (object->velocity[1] + (gravity * object->mass));

        for (int x = 0; x < size; x++) {
            if (x == i) {
                continue;
            }

            if (check_collision(object, &objects[x])) {
                return;
            }
        }

        object->velocity[1] = velocity_y;

        update_position(object);
    }
}

void update_position(GameObject* object) {
    object->position[0] += object->velocity[0];
    object->position[1] += object->velocity[1];
}

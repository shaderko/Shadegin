#include "collider.h"
#include "game_object.h"
#include <stdlib.h>
#include <stdbool.h>

#include "../render/render.h"

static bool check_collision_square_square(GameObject *object1, GameObject *object2) {
    SquareCollider square1 = object1->collider->data.square;
    SquareCollider square2 = object2->collider->data.square;

    vec3 pos1;
    memcpy(pos1, object1->position, sizeof(vec3));
    vec3 pos2;
    memcpy(pos2, object2->position, sizeof(vec3));

    // Calculate the half-width and half-height for both squares
    float half_width1 = square1.size[0] / 2.0f;
    float half_height1 = square1.size[1] / 2.0f;
    float half_depth1 = square1.size[2] / 2.0f;

    float half_width2 = square2.size[0] / 2.0f;
    float half_height2 = square2.size[1] / 2.0f;
    float half_depth2 = square2.size[2] / 2.0f;

    // Check if the squares are overlapping along the x-axis
    bool overlap_x = fabs(pos1[0] - pos2[0]) < (half_width1 + half_width2);

    // Check if the squares are overlapping along the y-axis
    bool overlap_y = fabs(pos1[1] - pos2[1]) < (half_height1 + half_height2);

    bool overlap_z = fabs(pos1[2] - pos2[2]) < (half_depth1 + half_depth2);

    // If the squares are overlapping along both the x and y axes, they are colliding
    return overlap_x && overlap_y && overlap_z;
}

static bool check_collision_square_circle(GameObject* object1, GameObject* object2) {
    // TODO:
    return 0;
}

static bool check_collision_circle_circle(GameObject* object1, GameObject* object2) {
    // TODO:
    return 0;
}

static CollisionCheckFunction collision_function_table[][3] = {
    // None Collider
    {NULL, NULL, NULL},
    // Square Collider
    {NULL, check_collision_square_square, check_collision_square_circle},
    // Circle Collider
    {NULL, check_collision_square_circle, check_collision_circle_circle}
};

static void collider_square_init(Collider* collider, const void* init_data) {
    collider->type = COLIDER_SQUARE;
    collider->data.square = *(const SquareCollider *)init_data;
}

static void collider_circle_init(Collider* collider, const void* init_data) {
    collider->type = COLIDER_CIRCLE;
    collider->data.circle = *(const CircleCollider *)init_data;
}

static collision_init_function collision_init_functions[3] = {NULL, collider_square_init, collider_circle_init};

Collider* collider_init(ColliderType type, const void *init_data) {
    if (type < 0 || type >= sizeof(collision_init_functions) / sizeof(collision_init_function)) {
        return NULL; // Invalid collider type
    }

    Collider* collider = malloc(sizeof(Collider));

    collision_init_function init_func = collision_init_functions[type];
    if (init_func) {
        init_func(collider, init_data);
    }

    return collider;
}

int check_collision(GameObject* object1, GameObject* object2) {
    ColliderType type1 = object1->collider->type;
    ColliderType type2 = object2->collider->type;

    // Get the appropriate collision function from the table
    CollisionCheckFunction collision_func = collision_function_table[type1][type2];

    if (collision_func) {
        return collision_func(object1, object2);
    }

    // Return 0 (no collision) for unsupported collider type combinations or NULL function pointers
    return 0;
}

void render_collider(GameObject* object) {
    render_square(object->position, (vec3){object->collider->data.square.size[0] + 1, object->collider->data.square.size[1] + 1, object->collider->data.square.size[2] + 1}, (vec4){0, 1, 0, 1}, false);
}

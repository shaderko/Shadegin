#include "collider.h"
#include "game_object.h"
#include <stdlib.h>
#include <stdbool.h>

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

    float half_width2 = square2.size[0] / 2.0f;
    float half_height2 = square2.size[1] / 2.0f;

    // Check if the squares are overlapping along the x-axis
    bool overlap_x = fabs(pos1[0] - pos2[0]) < (half_width1 + half_width2);

    // Check if the squares are overlapping along the y-axis
    bool overlap_y = fabs(pos1[1] - pos2[1]) < (half_height1 + half_height2);

    // If the squares are overlapping along both the x and y axes, they are colliding
    return overlap_x && overlap_y;
}

static bool check_collision_square_circle(GameObject* object1, GameObject* object2) {
    vec3 squarePos;
    vec3 circlePos;
    vec3 squareSize;
    float circleRadius;
    if (object1->collider->type == COLIDER_SQUARE) {
        memcpy(squarePos, object1->position, sizeof(vec3));
        memcpy(circlePos, object2->position, sizeof(vec3));
        memcpy(squareSize, object1->collider->data.square.size, sizeof(vec3));
        circleRadius = object2->collider->data.circle.radius;
    } else {
        memcpy(squarePos, object2->position, sizeof(vec3));
        memcpy(circlePos, object1->position, sizeof(vec3));
        memcpy(squareSize, object2->collider->data.square.size, sizeof(vec3));
        circleRadius = object1->collider->data.circle.radius;
    }

    // Calculate the closest point on the square to the circle's center
    float closestX = fmax(squarePos[0] - squareSize[0] * .5, fmin(circlePos[0], squarePos[0] + squareSize[0] * .5));
    float closestY = fmax(squarePos[1] - squareSize[1] * .5, fmin(circlePos[1], squarePos[1] + squareSize[1] * .5));

    // Calculate the squared distance between the closest point and the circle's center
    float distanceSquared = pow(closestX - circlePos[0], 2) + pow(closestY - circlePos[1], 2);

    printf("collision square circle detected\n");

    return distanceSquared <= pow(circleRadius, 2);
}

static bool check_collision_circle_circle(GameObject* object1, GameObject* object2) {
    vec3 pos1;
    vec3 pos2;
    memcpy(pos1, object1->position, sizeof(vec3));
    memcpy(pos2, object2->position, sizeof(vec3));
    float radius1 = object1->collider->data.circle.radius;
    float radius2 = object2->collider->data.circle.radius;

    float distanceSquared = pow(pos2[0] - pos1[0], 2) + pow(pos2[1] - pos1[1], 2);
    float radiusSumSquared = pow(radius1 + radius2, 2);

    return distanceSquared <= radiusSumSquared;
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

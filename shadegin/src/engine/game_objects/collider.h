#pragma once

#include <linmath.h>
#include <stdbool.h>

typedef struct gameobject GameObject;

typedef enum {
    COLIDER_NONE,
    COLIDER_SQUARE,
    COLIDER_CIRCLE,
    // Add more collider structures here
} ColliderType;

typedef struct {
    vec3 size;
} SquareCollider;

typedef struct {
    float radius;
} CircleCollider;

typedef struct collider {
    ColliderType type;
    union {
        SquareCollider square;
        CircleCollider circle;
        // Add more collider structures here
    } data;
} Collider;

typedef bool (*CollisionCheckFunction)(GameObject *object1, GameObject *object2);
typedef void (*collision_init_function)(Collider* collider, const void* init_data);

Collider* collider_init(ColliderType type, const void *init_data);
int check_collision(GameObject* object1, GameObject* object2);

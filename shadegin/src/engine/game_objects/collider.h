/**
 * Collider
 *
 * Collisions used for physics
 */

#ifndef COLLIDER_H
#define COLLIDER_H

typedef enum
{
    Collider_NULL,
    Collider_BOX,
    Collider_SPHERE,
} ColliderType;

typedef struct BoxCollider BoxCollider;
typedef struct SphereCollider SphereCollider;

struct BoxCollider
{
    float width;
    float height;
    float depth;
};

struct SphereCollider
{
    float radius;
};

typedef struct Collider Collider;
struct Collider
{
    /**
     * Used to specify which collision is used
     */
    ColliderType type;

    /**
     *
     */
    union
    {
        BoxCollider box;
        SphereCollider sphere;
    } data;
};

#endif
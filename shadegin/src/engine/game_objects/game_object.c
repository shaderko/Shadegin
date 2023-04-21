/**
 * @file game_object.c
 * @author https://github.com/shaderko
 * @brief Game object is used to simulate physics and collisions, and is the main object in the game
 * @date 2023-04-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "game_object.h"
#include "collider/collider.h"
#include "renderer/renderer.h"

static GameObject *Init()
{
    GameObject *object = malloc(sizeof(GameObject));

    return object;
}

/**
 * Initialize a box game object with box collider and renderer
 *
 * @param is_static dynamic physics or static
 * @return GameObject*
 */
static GameObject *InitBox(bool is_static, float mass, vec3 position, vec3 size)
{
    GameObject *object = Init();

    /**
     * Initialize all the main variables like is_static...
     */
    memcpy(object->position, position, sizeof(vec3));
    object->mass = mass;
    object->is_static = is_static;

    object->collider = ACollider->InitBox((vec3){0, 0, 0}, size);
    object->renderer = ARenderer->InitBox((vec3){0, 0, 0}, size);
    return object;
}

static void Render(GameObject *object)
{
    object->renderer->Render(object->renderer, object->position);
}

static void ApplyGravity(GameObject *object)
{
    object->velocity[1] += -.01;
}

static void Update(GameObject *object)
{
    if (object->is_static || object->collider->Collide(object->collider))
    {
        return;
    }

    ApplyGravity(object);

    object->position[0] += object->velocity[0];
    object->position[1] += object->velocity[1];
    object->position[2] += object->velocity[2];
}

struct AGameObject AGameObject[1] =
    {{Init,
      InitBox,
      Render,
      Update}};
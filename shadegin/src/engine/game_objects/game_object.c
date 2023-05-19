/**
 * @file game_object.c
 * @author https://github.com/shaderko
 * @brief Game object is used to simulate physics and collisions, and is the main object in the game
 * @date 2023-04-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../util.h"
#include "game_object.h"
#include "collider/collider.h"
#include "renderer/renderer.h"

static GameObject **GameObjectsArray = NULL;
static size_t GameObjectsSize = 0;

static GameObject *Init()
{
    GameObject *object = malloc(sizeof(GameObject));
    if (object == NULL)
    {
        ERROR_EXIT("GameObject memory couldn't be allocated!\n");
    }
    object->id = generate_random_id();
    printf("Initialized object with id %d\n", object->id);

    GameObjectsArray = realloc(GameObjectsArray, (GameObjectsSize + 1) * sizeof(GameObject *));
    if (GameObjectsArray == NULL)
    {
        ERROR_EXIT("GameObjectsArray memory couldn't be allocated!\n");
    }
    GameObjectsArray[GameObjectsSize] = object;
    GameObjectsSize++;

    return object;
}

/**
 * Initialize a box game object with no collider and renderer
 *
 * @param is_static dynamic physics or static
 * @return GameObject*
 */
static GameObject *Create(bool is_static, float mass, vec3 position)
{
    GameObject *object = Init();

    /**
     * Initialize all the main variables like is_static...
     */
    memcpy(object->position, position, sizeof(vec3));
    object->mass = mass;
    object->is_static = is_static;

    object->collider = NULL;
    object->renderer = NULL;
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

/**
 * @brief Render the game object
 *
 * @param object
 */
static void Render(GameObject *object)
{
    object->renderer->Render(object->renderer, object->position);
}

/**
 * @brief Render all game objects
 *
 */
static void RenderGameObjects()
{
    for (int x = 0; x < GameObjectsSize; x++)
    {
        Render(GameObjectsArray[x]);
    }
}

/**
 * @brief Apply gravity to object
 *
 * @param object
 */
static void ApplyGravity(GameObject *object)
{
    object->velocity[1] += -.01;
}

/**
 * @brief Update the game object and it's physics and collisions
 *
 * @param object
 */
static void Update(GameObject *object)
{
    if (object->is_static)
    {
        return;
    }

    for (int i = 0; i < GameObjectsSize; i++)
    {
        if (object->collider->Collide(object, GameObjectsArray[i]))
        {
            return;
        }
    }

    ApplyGravity(object);

    object->position[0] += object->velocity[0];
    object->position[1] += object->velocity[1];
    object->position[2] += object->velocity[2];
}

/**
 * @brief Update all game objects
 *
 */
static void UpdateGameObjects()
{
    for (int x = 0; x < GameObjectsSize; x++)
    {
        Update(GameObjectsArray[x]);
    }
}

/**
 * @brief Serialize the game object
 *
 * @param object
 * @return SerializedDerived
 */
static SerializedDerived Serialize(GameObject *object)
{
    SerializedCollider collider = ACollider->Serialize(object->collider);
    SerializedRenderer renderer = ARenderer->Serialize(object->renderer);

    SerializedGameObject serialize_obj = {
        object->id,
        {object->position[0], object->position[1], object->position[2]},
        {object->velocity[0], object->velocity[1], object->velocity[2]},
        object->mass,
        object->is_static,
        collider,
        renderer};

    SerializedDerived result;
    result.len = sizeof(SerializedGameObject) + collider.derived.len + renderer.derived.len;
    result.data = malloc(result.len);
    memcpy(result.data, &serialize_obj, sizeof(SerializedGameObject));
    memcpy(result.data + sizeof(SerializedGameObject), serialize_obj.collider.derived.data, serialize_obj.collider.derived.len);
    memcpy(result.data + sizeof(SerializedGameObject) + serialize_obj.collider.derived.len, serialize_obj.renderer.derived.data, serialize_obj.renderer.derived.len);

    free(serialize_obj.collider.derived.data);
    free(serialize_obj.renderer.derived.data);

    return result;
}

/**
 * @brief Deserialize the game object, update it's variables and create a new one if it doesn't exist
 *
 * @param object
 * @param collider
 * @param renderer
 * @return GameObject*
 */
static GameObject *Deserialize(SerializedGameObject *object, int *collider, int *renderer)
{
    for (int x = 0; x < GameObjectsSize; x++)
    {
        if (GameObjectsArray[x]->id == object->id)
        {
            printf("assigning position!\n");
            GameObjectsArray[x]->position[0] = object->position[0];
            GameObjectsArray[x]->position[1] = object->position[1];
            GameObjectsArray[x]->position[2] = object->position[2];
            printf("position updated %f, %f\n", object->position[0], object->position[1]);
            return GameObjectsArray[x];
        }
    }
    printf("object wans't found, creating\n");

    GameObject *new_obj = AGameObject->Create(object->is_static, object->mass, object->position);
    new_obj->id = object->id;
    switch (object->collider.type)
    {
    case BOX_COLLIDER:
        new_obj->collider = ACollider->InitBox(object->collider.position, collider);
        break;
    default:
        break;
    }

    switch (object->renderer.type)
    {
    case BOX_RENDERER:
        new_obj->renderer = ARenderer->InitBox(object->renderer.position, renderer);
        break;
    default:
        break;
    }

    printf("object created!\n");
    return new_obj;
}

struct AGameObject AGameObject[1] =
    {{Init,
      Create,
      InitBox,
      Render,
      RenderGameObjects,
      Update,
      UpdateGameObjects,
      Serialize,
      Deserialize}};
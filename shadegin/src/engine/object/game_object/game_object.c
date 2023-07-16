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

#include "../../util/util.h"
#include "../collider/collider.h"
#include "../renderer/renderer.h"
#include "../map/scene.h"

/**
 * All game object array (all that exist, so that means even in multiple rooms)
 */
static GameObject **GameObjectsArray = NULL;
static size_t GameObjectsSize = 0;

static GameObject *Init()
{
    GameObject *object = malloc(sizeof(GameObject));
    if (!object)
    {
        ERROR_EXIT("GameObject memory couldn't be allocated!\n");
    }

    object->id = generate_random_id();
    printf("Initialized object with id %llu\n", object->id);

    GameObjectsArray = realloc(GameObjectsArray, (GameObjectsSize + 1) * sizeof(GameObject *));
    if (!GameObjectsArray)
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
 * @brief Initialize a box game object with box collider and renderer
 *
 * @param is_static dynamic or static physics
 * @return GameObject*
 */
static GameObject *InitBox(bool is_static, float mass, vec3 position, vec3 size)
{
    GameObject *object = AGameObject->Create(is_static, mass, position);

    object->collider = ACollider->InitBox((vec3){0, 0, 0}, size);
    object->renderer = ARenderer->InitBox((vec3){0, 0, 0}, (vec3){0, 0, 0}, size);

    return object;
}

static GameObject *InitMesh(bool is_static, float mass, vec3 position, vec3 size, Model *model)
{
    GameObject *object = AGameObject->Create(is_static, mass, position);

    object->collider = ACollider->InitBox((vec3){0, 0, 0}, size);
    object->renderer = ARenderer->InitMesh(model, (vec4){1, 1, 1, 1}, (vec3){0, 0, 0}, (vec3){0, 0, 0}, size);

    return object;
}

/**
 * @brief Get the game object by index of object in array
 *
 * @param index index of the game object in the array
 * @return GameObject* or NULL if the game object doesn't exist
 */
static GameObject *GetGameObjectByIndex(int index)
{
    if (index >= GameObjectsSize || index < 0)
    {
        return NULL;
    }
    return GameObjectsArray[index];
}

/**
 * @brief Render the game object
 *
 * @param object object to be rendered
 */
static void Render(GameObject *object)
{
    ARenderer->Render(object->renderer, object->position);
}

/**
 * @brief Render all game objects
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
 * @param object to apply gravity to
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
    if (!result.data)
    {
        free(collider.derived.data);
        free(renderer.derived.data);
        ERROR_EXIT("SerializedDerived memory couldn't be allocated!\n");
    }

    memcpy((char *)result.data, &serialize_obj, sizeof(SerializedGameObject));
    memcpy((char *)result.data + sizeof(SerializedGameObject), (char *)collider.derived.data, collider.derived.len);

    // Renderer
    memcpy((char *)result.data + sizeof(SerializedGameObject) + collider.derived.len, (char *)renderer.derived.data, renderer.derived.len);

    free(collider.derived.data);
    free(renderer.derived.data);

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
static GameObject *Deserialize(SerializedGameObject *object, Scene *scene)
{
    printf("Deserializing game object with id %llu\n", object->id);
    if (!scene)
    {
        for (int x = 0; x < GameObjectsSize; x++)
        {
            if (GameObjectsArray[x]->id == object->id)
            {
                GameObjectsArray[x]->position[0] = object->position[0];
                GameObjectsArray[x]->position[1] = object->position[1];
                GameObjectsArray[x]->position[2] = object->position[2];
                GameObjectsArray[x]->velocity[0] = object->velocity[0];
                GameObjectsArray[x]->velocity[1] = object->velocity[1];
                GameObjectsArray[x]->velocity[2] = object->velocity[2];
                return NULL;
            }
        }
    }
    else
    {
        for (int x = 0; x < scene->objects_size; x++)
        {
            if (scene->objects[x]->id == object->id)
            {
                scene->objects[x]->position[0] = object->position[0];
                scene->objects[x]->position[1] = object->position[1];
                scene->objects[x]->position[2] = object->position[2];
                scene->objects[x]->velocity[0] = object->velocity[0];
                scene->objects[x]->velocity[1] = object->velocity[1];
                scene->objects[x]->velocity[2] = object->velocity[2];
                return NULL;
            }
        }
    }

    GameObject *new_obj = AGameObject->Create(object->is_static, object->mass, object->position);
    new_obj->id = object->id;
    switch (object->collider.type)
    {
    case BOX_COLLIDER:
        new_obj->collider = ACollider->InitBox((float *)object->collider.position, (float *)&(vec3){100, 100, 100});
        break;
    default:
        break;
    }

    printf("object->renderer.derived.len: %d\n", object->renderer.derived.len);

    new_obj->renderer = ARenderer->Deserialize(object->renderer);

    return new_obj;
}

struct AGameObject AGameObject[1] =
    {{
        Init,
        Create,
        InitBox,
        InitMesh,
        GetGameObjectByIndex,
        Render,
        RenderGameObjects,
        Update,
        UpdateGameObjects,
        Serialize,
        Deserialize,
    }};
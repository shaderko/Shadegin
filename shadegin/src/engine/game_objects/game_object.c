/**
 * @file game_object.c
 * @author https://github.com/shaderko
 * @brief Game object is used to simulate physics and collisions
 * @date 2023-04-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "game_object.h"
#include "box_collider.h"

static GameObject *Init()
{
    GameObject *object = malloc(sizeof(GameObject));


    object->position_x = 10;

    return object;
}

GameObject* InitBox(float width, float height, float depth) {
    GameObject* object = Init();
    object->collider = ABoxCollider->Init(width, height, depth);
    return object;
}

GameObject* InitSphere(float radius) {
    // object->collider = ABoxCollider->Init(10, 10, 10);
    return Init();
}

static void Update(GameObject *object)
{
    object->position_x += 1;
}

struct AGameObject AGameObject[1] =
    {{Init,
      InitBox,
      InitSphere,
      Update}};
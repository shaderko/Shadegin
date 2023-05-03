/**
 * @file scene.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-05-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include <linmath.h>

#include "../game_object.h"

typedef struct Scene Scene;
struct Scene
{
    /**
     * Defines bounds of a scene
     */
    vec3 size;

    GameObject **objects;
    int objects_size;
};

struct AScene
{
    Scene *(*Init)(vec3 *);
    void (*Add)(Scene *scene, GameObject *object);
    void (*WriteToFile)(Scene *scene, const char *file);
    void (*ReadFile)(Scene *scene, const char *file);
};

extern struct AScene AScene[1];

#endif
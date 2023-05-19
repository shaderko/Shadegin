/**
 * @file scene.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-05-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>

#include "scene.h"
#include "../../util.h"
#include "../game_object.h"

static Scene *Init(vec3 *size)
{
    Scene *scene = malloc(sizeof(Scene));
    if (scene == NULL)
    {
        ERROR_EXIT("Scene memory couldn't be allocated!\n");
    }

    memcpy(scene->size, size, sizeof(vec3));
    scene->objects = NULL;
    scene->objects_size = 0;

    return scene;
}

static Update(Scene *scene)
{
    for (int i = 0; i < scene->objects_size; i++)
    {
        AGameObject->Update(scene->objects[i]);
    }
}

static void Add(Scene *scene, GameObject *object)
{
    scene->objects = realloc(scene->objects, sizeof(GameObject *) * (scene->objects_size + 1));
    scene->objects[scene->objects_size] = object;
    scene->objects_size++;
}

static void WriteToFile(Scene *scene, const char *file)
{
    FILE *out = fopen(file, "wb");
    if (out == NULL)
    {
        printf("Error opening file: %s\n", file);
        return;
    }

    // Write the total number of objects
    fwrite(&scene->objects_size, sizeof(int), 1, out);

    // Write objects data
    for (int i = 0; i < scene->objects_size; i++)
    {
        GameObject *object = scene->objects[i];

        SerializedDerived serialized_object = AGameObject->Serialize(object);
        fwrite(serialized_object.data, serialized_object.len, 1, out);
    }

    fclose(out);
}

static void ReadFile(Scene *scene, const char *file)
{
    FILE *in = fopen(file, "rb");
    if (in == NULL)
    {
        printf("Error opening file: %s\n", file);
        return;
    }

    int size;
    fread(&size, sizeof(int), 1, in);

    // Read objects data
    printf("reading objects\n");
    for (int i = 0; i < size; i++)
    {
        SerializedGameObject data;
        fread(&data, sizeof(SerializedGameObject), 1, in);

        SerializedDerived collider = data.collider.derived;
        SerializedDerived renderer = data.renderer.derived;

        int *ad_data = malloc(data.collider.derived.len + data.renderer.derived.len);

        fread(ad_data, data.collider.derived.len + data.renderer.derived.len, 1, in);
        AScene->Add(scene, AGameObject->Deserialize(&data, ad_data, ad_data + data.collider.derived.len));

        free(ad_data);
    }

    printf("finished reading\n");
    fclose(in);
}

struct AScene AScene[1] =
    {{
        Init,
        Update,
        Add,
        WriteToFile,
        ReadFile,
    }};
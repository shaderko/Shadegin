/**
 * @file renderer.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-04-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "renderer.h"
#include "box_renderer.h"
#include "mesh_renderer.h"
#include "../../util/util.h"
#include "../model/model.h"

static void Delete(Renderer *renderer)
{
    free(renderer);
}

static Renderer *Init()
{
    Renderer *renderer = malloc(sizeof(Renderer));
    if (!renderer)
    {
        ERROR_EXIT("error allocating memory for collider.");
    }

    renderer->Delete = Delete;

    return renderer;
}

static Renderer *InitBox(vec3 position, vec3 size)
{
    Renderer *renderer = malloc(sizeof(Renderer));
    if (!renderer)
    {
        ERROR_EXIT("Error allocating memory for renderer.");
    }

    memcpy(renderer->position, position, sizeof(vec3));
    ABoxRenderer->Init(renderer, size);

    return renderer;
}

static Renderer *InitMesh(Model *model)
{
    Renderer *renderer = malloc(sizeof(Renderer));
    if (!renderer)
    {
        ERROR_EXIT("Error allocating memory for renderer.");
    }

    AMeshRenderer->Init(renderer, (vec3){0, 0, 0}, (vec3){0, 0, 0}, (vec3){1, 1, 1}, model);

    return renderer;
}

static SerializedRenderer Serialize(Renderer *renderer)
{
    SerializedRenderer serialized = {
        {renderer->position[0], renderer->position[1], renderer->position[2]},
        renderer->type,
        renderer->Seralize(renderer),
    };
    return serialized;
}

struct ARenderer ARenderer[1] =
    {{Init,
      InitBox,
      InitMesh,
      Serialize}};
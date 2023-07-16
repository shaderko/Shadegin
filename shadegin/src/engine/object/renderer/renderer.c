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

#include "../../util/util.h"
#include "../../render/render/render.h"
#include "../model/model.h"

static void Delete(Renderer *renderer)
{
    AModel->Delete(renderer->model);
    free(renderer);
}

static Renderer *Init(vec3 position, vec3 rotation, vec3 scale)
{
    Renderer *renderer = malloc(sizeof(Renderer));
    if (!renderer)
    {
        ERROR_EXIT("error allocating memory for collider.");
    }

    memcpy(renderer->position, position, sizeof(vec3));
    memcpy(renderer->rotation, rotation, sizeof(vec3));
    memcpy(renderer->scale, scale, sizeof(vec3));

    return renderer;
}

static void Render(Renderer *renderer, vec3 position)
{
    if (!renderer || !renderer->model)
    {
        return;
    }
    vec3 world_position = {0, 0, 0};
    vec3_add(world_position, position, renderer->position);
    render_mesh(renderer->model, world_position, renderer->scale); // renderer->rotation, renderer->scale
}

static Renderer *InitBox(vec3 position, vec3 rotation, vec3 scale)
{
    Renderer *renderer = ARenderer->Init(position, rotation, scale);

    renderer->model = AModel->InitBox();

    return renderer;
}

static Renderer *InitMesh(Model *model, vec4 color, vec3 position, vec3 rotation, vec3 scale)
{
    Renderer *renderer = ARenderer->Init(position, rotation, scale);

    renderer->model = model;

    return renderer;
}

static SerializedRenderer Serialize(Renderer *renderer)
{
    SerializedRenderer serialized = {0};

    memcpy(serialized.position, renderer->position, sizeof(vec3));
    memcpy(serialized.rotation, renderer->rotation, sizeof(vec3));
    memcpy(serialized.scale, renderer->scale, sizeof(vec3));

    serialized.type = renderer->type;
    serialized.derived = AModel->Serialize(renderer->model);

    return serialized;
}

static Renderer *Deserialize(SerializedRenderer serialized)
{
    Renderer *renderer = malloc(sizeof(Renderer));

    memcpy(renderer->position, serialized.position, sizeof(vec3));
    memcpy(renderer->rotation, serialized.rotation, sizeof(vec3));
    memcpy(renderer->scale, serialized.scale, sizeof(vec3));

    renderer->type = serialized.type;
    renderer->model = AModel->Deserialize(serialized.derived);

    return renderer;
}

struct ARenderer ARenderer[1] =
    {{
        Init,
        Render,
        InitBox,
        InitMesh,
        Serialize,
        Deserialize,
    }};
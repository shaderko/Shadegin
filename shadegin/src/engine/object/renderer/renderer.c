/**
 * @file renderer.c
 * @author https://github.com/shaderko
 * @brief Used to render a model
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
        printf("Couldn't render, renderer NULL, or model NULL!\n");
        return;
    }

    // Adjust the position to be relative to world space by adding the position of the renderer to the position of the object
    vec3 world_position = {0, 0, 0};
    vec3_add(world_position, position, renderer->position);

    render_mesh(renderer->model, world_position, renderer->scale); // renderer->rotation, renderer->scale TODO:
}

/**
 * @brief Initialize a cube renderer
 *
 * @param position - vec3 of renderer position
 * @param rotation - vec3 of renderer rotation
 * @param scale - vec3 of renderer scale
 * @return Renderer*
 */
static Renderer *InitBox(vec3 position, vec3 rotation, vec3 scale)
{
    Renderer *renderer = ARenderer->Init(position, rotation, scale);

    renderer->model = AModel->InitBox();

    return renderer;
}

/**
 * @brief Initialize a mesh renderer
 *
 * @param model - initialized model
 * @param color - color of the mesh // TODO:
 * @param position - vec3 of renderer position
 * @param rotation - vec3 of renderer rotation
 * @param scale - vec3 of renderer scale
 * @return Renderer*
 */
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

    serialized.derived = AModel->Serialize(renderer->model);

    return serialized;
}

static Renderer *Deserialize(SerializedRenderer serialized)
{
    Renderer *renderer = malloc(sizeof(Renderer));

    memcpy(renderer->position, serialized.position, sizeof(vec3));
    memcpy(renderer->rotation, serialized.rotation, sizeof(vec3));
    memcpy(renderer->scale, serialized.scale, sizeof(vec3));

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
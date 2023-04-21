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
#include "../../util.h"
#include "box_renderer.h"

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
    memcpy(renderer->position, (vec3){0, 0, 0}, sizeof(vec3));
    renderer->Delete = Delete;

    return renderer;
}

static Renderer *InitBox(vec3 position, vec3 size)
{
    Renderer *renderer = malloc(sizeof(Renderer));
    if (!renderer)
    {
        ERROR_EXIT("error allocating memory for dlider.");
    }

    ABoxRenderer->Init(renderer, position, size);

    return renderer;
}

struct ARenderer ARenderer[1] =
    {{Init,
      InitBox}};
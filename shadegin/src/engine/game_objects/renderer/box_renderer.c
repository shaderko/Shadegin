/**
 * @file box_renderer.c
 * @author https://github.com/shaderko
 * @brief TODO:
 * @version 0.1
 * @date 2023-04-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "box_renderer.h"
#include "../../render/render.h"

static void Delete(Renderer *renderer)
{
    free(renderer->derived);
    free(renderer);
}

static vec3 *Size(Renderer *renderer)
{
    return NULL;
    // return renderer->derived->size;
}

static void Render(Renderer *renderer)
{
    vec3 world_position = {50, 50, 0};
    // vec3_add(world_position, renderer->derived->position, renderer->position);
    render_square(world_position, (vec3){50, 50, 50}, (vec4){0, 1, 1, 1}, true);
}

static Renderer *Init(Renderer *renderer, vec3 position, vec3 size)
{
    BoxRenderer *box_renderer;
    box_renderer = malloc(sizeof(BoxRenderer));
    if (box_renderer == NULL)
    {
        renderer->Delete(renderer);
        return NULL; // TODO: error exit
    }
    renderer->derived = box_renderer;
    memcpy(renderer->color, (vec4){1, 1, 1, 1}, sizeof(renderer->color));
    // renderer->color = {1, 1, 1, 1};
    box_renderer->parent = renderer;

    // box_renderer->size = size;
    // box_renderer->position = position;

    renderer->Render = Render;
    renderer->Size = Size;
    renderer->Delete = Delete;

    return renderer;
}

struct ABoxRenderer ABoxRenderer[1] =
    {{Init}};
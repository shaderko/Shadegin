/**
 * @file box_renderer.c
 * @author https://github.com/shaderko
 * @brief Used for rendering a 3D box
 * @version 0.1
 * @date 2023-04-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../../util/util.h"
#include "box_renderer.h"
#include "../../render/render/render.h"

static void Delete(Renderer *renderer)
{
    free(renderer->derived);
    free(renderer);
}

static vec3 *Size(Renderer *renderer)
{
    static vec3 defaultSize = {0, 0, 0};
    if (renderer == NULL || renderer->derived == NULL)
    {
        return &defaultSize;
    };

    BoxRenderer *derived_renderer = (BoxRenderer *)renderer->derived;
    return &derived_renderer->size;
}

static void Render(Renderer *renderer, vec3 position)
{
    vec3 world_position = {0, 0, 0};
    vec3_add(world_position, position, renderer->position);
    render_square(world_position, *renderer->Size(renderer), (vec4){0, 0, 0, 0}, true); // TODO: color
}

static SerializedDerived Serialize(Renderer *renderer)
{
    SerializedDerived serialize_renderer;
    serialize_renderer.len = sizeof(vec3);
    serialize_renderer.data = malloc(serialize_renderer.len);
    if (!serialize_renderer.data)
    {
        ERROR_EXIT("Couldn't allocate memory for serialized renderer!\n");
    }
    memcpy((char *)serialize_renderer.data, renderer->Size(renderer), serialize_renderer.len);

    return serialize_renderer;
}

static Renderer *Init(Renderer *renderer, vec3 size)
{
    BoxRenderer *box_renderer;
    box_renderer = malloc(sizeof(BoxRenderer));
    if (box_renderer == NULL)
    {
        renderer->Delete(renderer);
        return NULL; // TODO: error exit
    }
    renderer->derived = box_renderer;
    renderer->type = BOX_RENDERER;

    box_renderer->parent = renderer;
    memcpy(box_renderer->size, size, sizeof(vec3));

    renderer->Render = Render;
    renderer->Size = Size;
    renderer->Delete = Delete;
    renderer->Seralize = Serialize;

    return renderer;
}

struct ABoxRenderer ABoxRenderer[1] =
    {{Init}};
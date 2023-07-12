/**
 * @file mesh_renderer.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "mesh_renderer.h"

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

    MeshRenderer *derived_renderer = (MeshRenderer *)renderer->derived;
    return &derived_renderer->scale;
}

static void Render(Renderer *renderer, vec3 position)
{
    MeshRenderer *mesh_renderer = (MeshRenderer *)renderer->derived;

    if (!mesh_renderer->model)
    {
        printf("Mesh renderer model is NULL\n");
        return;
    }

    if (!mesh_renderer->model->is_valid)
    {
        printf("Mesh renderer model is not valid\n");
        return;
    }

    puts("rendering mesh");

    render_mesh(mesh_renderer->model);
}

// TODO:
static SerializedDerived Serialize(Renderer *renderer)
{
    SerializedDerived serialize_renderer;
    serialize_renderer.len = sizeof(renderer->derived->model);
    serialize_renderer.data = malloc(serialize_renderer.len);
    if (!serialize_renderer.data)
    {
        ERROR_EXIT("Couldn't allocate memory for serialized renderer!\n");
    }
    memcpy((char *)serialize_renderer.data, renderer->Size(renderer), serialize_renderer.len);

    return serialize_renderer;
}

static Renderer *Init(Renderer *renderer, vec3 position, vec3 rotation, vec3 scale, Model *model)
{
    MeshRenderer *mesh_renderer;
    mesh_renderer = malloc(sizeof(MeshRenderer));
    if (mesh_renderer == NULL)
    {
        renderer->Delete(renderer);
        return NULL; // TODO: error exit
    }
    renderer->derived = mesh_renderer;
    renderer->type = MESH_RENDERER;

    mesh_renderer->parent = renderer;
    mesh_renderer->model = model;
    memcpy(mesh_renderer->scale, scale, sizeof(vec3));

    renderer->Render = Render;
    renderer->Size = Size;
    renderer->Delete = Delete;
    renderer->Seralize = Serialize;

    return renderer;
}

struct AMeshRenderer AMeshRenderer[1] = {{
    Init,
}};
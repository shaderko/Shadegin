/**
 * @file mesh_renderer.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <linmath.h>

#include "renderer.h"
#include "../model/model.h"

typedef struct MeshRenderer MeshRenderer;
struct MeshRenderer
{
    Renderer *parent;

    vec3 position;
    vec3 rotation;
    vec3 scale;

    Model *model;
};

struct AMeshRenderer
{
    Renderer *(*Init)(Renderer *renderer, vec3 position, vec3 rotation, vec3 scale, Model *model);
};

extern struct AMeshRenderer AMeshRenderer[1];

#endif
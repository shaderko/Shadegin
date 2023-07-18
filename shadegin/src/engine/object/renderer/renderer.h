/**
 * @file renderer.h
 * @author https://github.com/shaderko
 * @brief Used to render a model
 * @version 0.1
 * @date 2023-07-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <linmath.h>

#include "../serialized/serialized.h"
#include "../model/model.h"

typedef struct SerializedRenderer SerializedRenderer;
struct SerializedRenderer
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
    SerializedDerived derived;
};

typedef struct Renderer Renderer;
struct Renderer
{
    /**
     * Local position to game object
     */
    vec3 position;

    vec3 rotation;

    vec3 scale;

    Model *model;
};

struct ARenderer
{
    /**
     * TODO:
     */
    Renderer *(*Init)(vec3 position, vec3 rotation, vec3 scale);
    void (*Render)(Renderer *renderer, vec3 position);
    Renderer *(*InitBox)(vec3 position, vec3 rotation, vec3 scale);
    Renderer *(*InitMesh)(Model *model, vec4 color, vec3 position, vec3 rotation, vec3 scale);

    SerializedRenderer (*Serialize)(Renderer *);
    Renderer *(*Deserialize)(SerializedRenderer serialized);
};

extern struct ARenderer ARenderer[1];

#endif
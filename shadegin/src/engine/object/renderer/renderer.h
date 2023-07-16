/*
 * Renderer used for rendering objects on the screen
 *
 * Used with GameObjects
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <linmath.h>

#include "../serialized/serialized.h"
#include "../model/model.h"

typedef enum RendererType RendererType;
enum RendererType
{
    NONE_RENDERER,
    BOX_RENDERER,
    MESH_RENDERER,
};

typedef struct SerializedRenderer SerializedRenderer;
struct SerializedRenderer
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
    RendererType type;
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

    /**
     * Renderer type for serialization
     */
    RendererType type;

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
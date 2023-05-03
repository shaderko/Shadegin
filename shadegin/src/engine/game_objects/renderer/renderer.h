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
#include "../serialized.h"

typedef enum RendererType RendererType;
enum RendererType
{
    NONE_RENDERER,
    BOX_RENDERER,
};

typedef struct SerializedRenderer SerializedRenderer;
struct SerializedRenderer
{
    vec3 position;
    RendererType type;
    SerializedDerived derived;
};

typedef struct Renderer Renderer;
struct Renderer
{
    /**
     * Pointer to the subclass
     */
    void *derived;

    /**
     * Local position to game object
     */
    vec3 position;

    /**
     * Renderer type for serialization
     */
    RendererType type;

    /**
     * Renders the Game Object with
     */
    void (*Render)(Renderer *, vec3);
    vec3 *(*Size)(Renderer *);
    void (*Delete)(Renderer *);
    SerializedDerived (*Seralize)(Renderer *);
};

struct ARenderer
{
    /**
     * TODO:
     */
    Renderer *(*Init)();
    Renderer *(*InitBox)(vec3 position, vec3 size);

    SerializedRenderer (*Serialize)(Renderer *);
};

extern struct ARenderer ARenderer[1];

#endif
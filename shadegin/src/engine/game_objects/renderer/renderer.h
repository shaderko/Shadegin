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
     * Renders the Game Object with
     */
    void (*Render)(Renderer *, vec3);
    vec3 *(*Size)(Renderer *);
    void (*Delete)(Renderer *);
};

struct ARenderer
{
    /**
     * TODO:
     */
    Renderer *(*Init)();
    Renderer *(*InitBox)(vec3 position, vec3 size);
};

extern struct ARenderer ARenderer[1];

#endif
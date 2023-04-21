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
     * Objects color (RGBA)
     */
    vec4 color;

    /**
     * Renders the Game Object with 
     */
    void (*Render)(Renderer *);
    vec3 *(*Size)(Renderer *);
    void (*Delete)(Renderer *);
};

struct ARenderer
{
    /**
     * Create game object
     *
     * Can have different collider and renderer types
     */
    Renderer *(*Init)();
    Renderer *(*InitBox)(vec3 position, vec3 size);
};

extern struct ARenderer ARenderer[1];

#endif
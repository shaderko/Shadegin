/*
 * Renderer used for rendering objects on the screen
 * 
 * Used with GameObjects
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <linmath.h>

typedef enum {
    RENDERER_NONE,
    RENDERER_SQUARE,
    RENDERER_CIRCLE,
} RendererType;

typedef struct {
    // vec3 position;
    vec3 size;
} SquareRenderer;

typedef struct {
    // vec3 position;
    float radius;
} CircleRenderer;

typedef struct renderer {
    RendererType type;
    union {
        SquareRenderer square;
        CircleRenderer circle;
    } data;
} Renderer;

// typedef void (*RendererInitFunctions)(Renderer* renderer, const void* init_data);
// typedef void (*RendererRenderFunctions)(GameObject* object);

// Renderer* renderer_init(RendererType type, const void *init_data);
// void renderer_render(GameObject* object);

#endif
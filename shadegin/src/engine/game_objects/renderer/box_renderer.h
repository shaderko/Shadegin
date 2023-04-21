/**
 * @file box_renderer.h
 * @author https://github.com/shaderko
 * @brief TODO:
 * @version 0.1
 * @date 2023-04-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef BOX_RENDERER_H
#define BOX_RENDERER_H

#include "renderer.h"
#include <linmath.h>

typedef struct BoxRenderer BoxRenderer;
struct BoxRenderer
{
    /**
     * TODO:
     */
    Renderer *parent;

    /**
     * Vector 3 size of Box Renderer
     */
    vec3 size;
};

struct ABoxRenderer
{
    /**
     * TODO:
     */
    Renderer *(*Init)(Renderer *renderer, vec3 position, vec3 size);
};

extern struct ABoxRenderer ABoxRenderer[1];

#endif
/**
 * @file render.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RENDER_H
#define RENDER_H

#define MIN_DEPTH_VALUE -0.0000001
#define GROUND_SIZE 0.0000001

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <linmath.h>
#include <stdbool.h>

#include "../../common/types/types.h"
#include "../camera/camera.h"

typedef struct render_state
{
    SDL_Window *window;
    f32 width;
    f32 height;
    f32 low_res_width;
    f32 low_res_height;
} Render_State;

void render_init(void);
void render_shaders(Camera *camera);
void render_begin(void);
void render_begin_pixelated(void);
void render_end(void);
void render_end_pixelated(void);
void render_update_projection(Camera *camera);
void render_light(vec3 position);
void render_quad(vec3 pos, vec3 size, vec4 color, bool fill);
void render_square(vec3 pos, vec3 size, vec4 color, bool fill);
void render_poly(vec2 *vertices, size_t num_vertices, vec4 color);
void render_line(vec2 src_pos, vec2 dst_pos, vec4 color);

#endif
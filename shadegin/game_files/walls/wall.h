#pragma once

#include <linmath.h>

typedef struct Wall {
    vec3 position;
    vec3 size;
    vec4 color;
} Wall;

void add_wall(vec3 position, vec3 wall_size, vec4 color);
void render_walls();

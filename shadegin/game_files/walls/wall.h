#pragma once

#include <linmath.h>
#include "../../src/engine/object/object.h"

typedef struct Wall
{
    vec3 position;
    vec3 size;
    vec4 color;
    Object **objects;
} Wall;

void add_wall(vec3 position, vec3 wall_size, vec4 color);
void render_walls();

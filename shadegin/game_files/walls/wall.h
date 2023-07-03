#pragma once

#include <linmath.h>
#include "../../src/engine/object/game_object/game_object.h"

typedef struct Wall
{
    vec3 position;
    vec3 size;
    vec4 color;
    GameObject **objects;
} Wall;

void add_wall(vec3 position, vec3 wall_size, vec4 color);
void render_walls();

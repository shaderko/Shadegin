#pragma once

#include <linmath.h>
#include "../../src/engine/game_objects/game_object.h"

typedef struct Wall {
    vec3 position;
    vec3 size;
    vec4 color;
    GameObject** objects;
} Wall;

void add_wall(vec3 position, vec3 wall_size, vec4 color);
void render_walls();

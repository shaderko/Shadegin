#ifndef WALL_H
#define WALL_H

#include <linmath.h>
#include <stdbool.h>

typedef struct wall {
    vec2 position;
    vec2 size;
    vec2* segments;
    bool render;
    vec2* rays;
    size_t rays_size;
} Wall;

Wall create_wall(float x, float y, float width, float height);
vec2* segments(Wall* walls, int size);
Wall random_wall(float width, float height);

#endif

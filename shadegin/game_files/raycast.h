#include <linmath.h>
#include "wall.h"

typedef struct raycast {
    float fov;
    vec3 color;
} Raycast;

Raycast raycast_init(float fov, vec3 color);
void sort_rays(vec2* rays, size_t rays_size, vec2 src_pos);
int cast_rays(Wall* walls, size_t walls_size, vec2 src_pos, int index, vec2* segments, size_t size);
void draw_rays(Wall* walls, size_t size, float src_x, float src_y);
vec2* intersection(float r_px, float r_py, float r_dx, float r_dy, vec2* segments, size_t segment_size);

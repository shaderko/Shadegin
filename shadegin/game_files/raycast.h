#include <linmath.h>

typedef struct raycast {
    float fov;
    vec3 color;
} Raycast;

Raycast raycast_init(float fov, vec3 color);
void sort_rays(vec2* rays, size_t rays_size, vec2 src_pos);
vec2* cast_rays(vec2* rays, size_t* rays_size, vec2 src_pos, vec2 dst_pos, vec2* segments, size_t size);
void draw_rays(vec2* rays, size_t size, float src_x, float src_y);
vec2* intersection(float r_px, float r_py, float r_dx, float r_dy, vec2* segments, size_t segment_size);

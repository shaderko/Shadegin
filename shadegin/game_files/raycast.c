#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <stdio.h>

#include "raycast.h"
#include "../src/engine/render/render.h"

#define radians(a) (a * 180.0 / M_PI)

Raycast raycast_init(float fov, vec3 color) {
    Raycast raycast = {fov, {255, 255, 255}};
    return raycast;
}

void sort_rays(vec2* rays, size_t rays_size, vec2 src_pos) {
    vec2* temp = malloc(sizeof(vec2));
    for (int i = 0; i < rays_size; i++) {
        double angle = atan2(rays[i][1] - src_pos[1], rays[i][0] - src_pos[0]);

        for (int x = 0; x < i; x++) {
            double angle_prev = atan2(rays[i - x - 1][1] - src_pos[1], rays[i - x - 1][0] - src_pos[0]);

            if (angle_prev > angle) {
                // swap
                memcpy(temp, &rays[i - x - 1], sizeof(vec2));
                memcpy(&rays[i - x - 1], &rays[i - x], sizeof(vec2));
                memcpy(&rays[i - x], temp, sizeof(vec2));
            } else {
                break;
            }
        }
    }
}

vec2* cast_rays(vec2* rays, size_t* rays_size, vec2 src_pos, vec2 dst_pos, vec2* segments, size_t size) {
    size_t segments_size = size * 4;

    float src_pos_x = src_pos[0];
    float src_pos_y = src_pos[1];

    float dst_pos_x = dst_pos[0];
    float dst_pos_y = dst_pos[1];

    vec2* lower = intersection(src_pos_x, src_pos_y, dst_pos_x - src_pos_x - radians(0.001), dst_pos_y - src_pos_y - radians(0.001), segments, segments_size);
    vec2* straight = intersection(src_pos_x, src_pos_y, dst_pos_x - src_pos_x, dst_pos_y - src_pos_y, segments, segments_size);
    vec2* higher = intersection(src_pos_x, src_pos_y, dst_pos_x - src_pos_x + radians(0.001), dst_pos_y - src_pos_y + radians(0.001), segments, segments_size);

    if (lower != NULL) {
        rays = realloc(rays, sizeof(vec2) * (*rays_size + 1));
        memcpy(rays + *rays_size, lower, sizeof(vec2));
        free(lower);
        (*rays_size)++;
    }
    if (straight != NULL) {
        rays = realloc(rays, sizeof(vec2) * (*rays_size + 1));
        memcpy(rays + *rays_size, straight, sizeof(vec2));
        free(straight);
        (*rays_size)++;
    }
    if (higher != NULL) {
        rays = realloc(rays, sizeof(vec2) * (*rays_size + 1));
        memcpy(rays + *rays_size, higher, sizeof(vec2));
        free(higher);
        (*rays_size)++;
    }

    return rays;
}

void draw_rays(vec2* rays, size_t size, float src_x, float src_y) {
    int i = 0;
    float increment = 1.f / (float)size;
    float color = 0;

    vec2* start_mouse = malloc(sizeof(vec2) * (size + 1));
    memcpy(start_mouse + 1, rays, sizeof(vec2) * size);
    memcpy(start_mouse, &(vec2){src_x, src_y}, sizeof(vec2));
    render_poly(start_mouse, size + 1, (vec3){0.215, 0.215, 0.215});
    // glLineWidth(10);

    while (i + 1 < size + 1) {
        color += increment;
        // render_line((vec2){src_x, src_y}, rays[i], (vec3){color, color, color});
        render_quad(rays[i], (vec2){5, 5}, (vec4){1, 0, 0, 1}, true);
        i++;
    }
}

vec2* intersection(float r_px, float r_py, float r_dx, float r_dy, vec2* segments, size_t segment_size) {
    bool is_null = true;
    vec2* closest_intersect = malloc(sizeof(vec2));
    float closest_distance = FLT_MAX;

    int index = 0;
    for (int i = 0; i < segment_size; i++) {
        float segment_src_x = segments[(i*2)][0];
        float segment_src_y = segments[(i*2)][1];
        float segment_dst_x = segments[(i*2) + 1][0];
        float segment_dst_y = segments[(i*2) + 1][1];

        float s_dx = segment_dst_x - segment_src_x;
        float s_dy = segment_dst_y - segment_src_y;

        float r_mag = sqrt(r_dx * r_dx + r_dy * r_dy);
        float s_mag = sqrt(s_dx * s_dx + s_dy * s_dy);
        if (r_dx / r_mag == s_dx / s_mag && r_dy / r_mag == s_dy / s_mag) {
            continue;
        }

        float T2 = (r_dx * (segment_src_y - r_py) + r_dy * (r_px - segment_src_x)) / (s_dx * r_dy - s_dy * r_dx);
        float T1 = (segment_src_x + s_dx * T2 - r_px) / r_dx;

        if (T1 < 0) {
            continue;
        }
        if (T2 < 0 || T2 > 1) {
            continue;
        }
        if (T1 < closest_distance) {
            closest_distance = T1;
            memcpy(closest_intersect, &(vec2){r_px + r_dx * T1, r_py + r_dy * T1}, sizeof(vec2));
            is_null = false;
        }
    }

    return is_null ? NULL : closest_intersect;
}

#pragma once

#include <linmath.h>

typedef struct camera {
    vec3 position;
    vec3 velocity;
    float distance;
    mat4x4 view;
    mat4x4* projection;
} Camera;

void camera_init(mat4x4* projection, float distance, vec3 position);
void camera_update_view();
void camera_update_position();
Camera* get_camera();

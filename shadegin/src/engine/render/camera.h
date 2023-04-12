#pragma once

#include <linmath.h>

typedef struct camera {
    vec3 position;
    vec3 velocity;
    float distance;
    float smoothing;
    mat4x4 view;
} Camera;

void camera_init(float distance, float smoothing, vec3 position);
void camera_update_view();
void camera_update_position(vec3 position);
Camera* get_camera();

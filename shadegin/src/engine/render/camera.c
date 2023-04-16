#include "camera.h"
#include "../global.h"
#include "../util.h"

Camera* camera = NULL;

void camera_init(float distance, float smoothing, vec3 position, GameObject* target) {
    camera = calloc(1, sizeof(Camera));
    if (camera == NULL) {
        ERROR_EXIT("error allocating memory for camera.\n");
    }
    camera->target = target;
    camera->distance = distance;
    camera->smoothing = smoothing;
    memcpy(camera->position, position, sizeof(vec3));
    camera_update_view();
}

Camera* get_camera() {
    if (camera == NULL) {
        ERROR_EXIT("error getting camera.\n");
    }
    return camera;
}

void camera_update_view() {
    vec3 eye = {camera->position[0], 50 + camera->position[1], 100};
    vec3 center = {camera->position[0], camera->position[1], 0};
    vec3 up = {0.0f, 1.0f, 0};
    mat4x4_look_at(camera->view, eye, center, up);

    render_shaders(camera);
}

void camera_follow_target() {
    if (camera->target) {
        camera_update_position((vec3){camera->target->position_x, camera->target->position_y, camera->target->position_z});
    }
}

void camera_update_position(vec3 position) {
    camera->velocity[0] = (position[0] - camera->position[0]) / camera->smoothing;
    camera->velocity[1] = (position[1] - camera->position[1]) / camera->smoothing;

    camera->position[0] += camera->velocity[0];
    camera->position[1] += camera->velocity[1];

    camera_update_view();
}

#include "camera.h"
#include "../global.h"

Camera* camera = NULL;

void camera_init(float distance, float smoothing, vec3 position) {
    camera = calloc(1, sizeof(Camera));
    if (camera == NULL) {
        // Error TODO:
        printf("ERROR");
        return;
    }
    camera->distance = distance;
    camera->smoothing = smoothing;
    memcpy(camera->position, position, sizeof(vec3));
    camera_update_view();
}

Camera* get_camera() {
    if (camera == NULL) {
        // Error TODO:
        printf("ERROR");
        exit(1);
    }
    return camera;
}

void camera_update_view() {
    vec3 eye = {camera->position[0], camera->position[1], 2};
    vec3 center = {camera->position[0], camera->position[1], 0};
    vec3 up = {0.0f, 1.0f, 0};
    mat4x4_look_at(camera->view, eye, center, up);

    render_shaders(camera);
}

void camera_update_position(vec3 position) {
    camera->velocity[0] = (position[0] - camera->position[0]) / camera->smoothing;
    camera->velocity[1] = (position[1] - camera->position[1]) / camera->smoothing;

    camera->position[0] += camera->velocity[0];
    camera->position[1] += camera->velocity[1];

    camera_update_view();
}

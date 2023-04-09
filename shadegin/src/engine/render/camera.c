#include "camera.h"
#include "../global.h"

Camera* camera = NULL;

void camera_init(mat4x4* projection, float distance, vec3 position) {
    camera = calloc(1, sizeof(Camera));
    if (camera == NULL) {
        // Error TODO:
        printf("ERROR");
        return;
    }
    camera->projection = projection;
    camera->distance = distance;
    memcpy(camera->position, position, sizeof(vec3));
    float aspect_ratio = (float)global.render.width / (float)global.render.height;
    mat4x4_ortho(*projection, -camera->distance * aspect_ratio, camera->distance * aspect_ratio, -camera->distance, camera->distance, -500, 500);
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
    vec3 eye = {camera->position[0], camera->position[1], camera->position[2]};
    printf("%f\n", camera->distance);
    vec3 center = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    mat4x4_look_at(camera->view, eye, center, up);
    float aspect_ratio = (float)global.render.width / (float)global.render.height;
    mat4x4_ortho(*camera->projection, -camera->distance * aspect_ratio, camera->distance * aspect_ratio, -camera->distance, camera->distance, -500, 500);
}

void camera_update_position(vec3 position) {
    camera->distance -= 100;
    camera->position[0] = position[0];
    camera->position[1] = position[1];
    camera->position[2] = position[2];

    camera_update_view();
}

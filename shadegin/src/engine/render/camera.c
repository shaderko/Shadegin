/**
 * @file camera.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-05-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "camera.h"
#include "../global.h"
#include "../util.h"

static Camera *camera = NULL;

static Camera *Init(float distance, float smoothing, vec3 position, GameObject *target)
{
    camera = malloc(sizeof(Camera));
    if (!camera)
    {
        ERROR_EXIT("error allocating memory for camera.\n");
    }

    camera->target = target;
    camera->distance = distance;
    camera->smoothing = smoothing;
    memcpy(camera->position, position, sizeof(vec3));
    mat4x4_identity(camera->view);
    ACamera->UpdateView(camera);

    return camera;
}

static Camera *Get()
{
    return camera;
}

static void UpdateView()
{
    int z_distance = 10000;
    vec3 eye = {camera->position[0], z_distance + camera->position[1], z_distance * 2};
    vec3 center = {camera->position[0], camera->position[1], 0};
    vec3 up = {0.0f, 1.0f, 0};
    mat4x4_look_at(camera->view, eye, center, up);

    render_shaders(camera);
}

static void FollowTarget()
{
    if (!camera->target)
    {
        return;
    }

    ACamera->UpdatePosition(camera, camera->target->position);
}

static void UpdatePosition(vec3 position)
{
    camera->velocity[0] = (position[0] - camera->position[0]) / camera->smoothing;
    camera->velocity[1] = (position[1] - camera->position[1]) / camera->smoothing;

    camera->position[0] += camera->velocity[0];
    camera->position[1] += camera->velocity[1];

    ACamera->UpdateView(camera);
}

struct ACamera ACamera[1] = {{
    Init,
    Get,
    UpdateView,
    FollowTarget,
    UpdatePosition,
}};
/**
 * @file camera.h
 * @author your name (you@domain.com)
 * @brief Camera for the scene, for now only one camera is supported
 * @version 0.1
 * @date 2023-05-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <linmath.h>

#include "../game_objects/game_object.h"

typedef struct Camera Camera;
struct Camera
{
    /**
     * @brief Position of the camera
     */
    vec3 position;

    /**
     * @brief Velocity of the camera
     */
    vec3 velocity;

    /**
     * @brief Distance from the target
     */
    float distance;

    /**
     * @brief Smoothing of the camera
     */
    float smoothing;

    /**
     * @brief Target of the camera
     */
    GameObject *target;

    /**
     * @brief View matrix of the camera
     */
    mat4x4 view;
};

struct ACamera
{
    /**
     * @brief Create camera
     */
    Camera *(*Init)(float distance, float smoothing, vec3 position, GameObject *target);

    /**
     * @brief Get camera
     */
    Camera *(*Get)();

    /**
     * @brief Update view matrix, used for rendering, so update after movement
     */
    void (*UpdateView)();

    /**
     * @brief Follow target, target must be set manually before calling this function
     */
    void (*FollowTarget)();

    /**
     * @brief Update position of the camera, also updates view matrix
     */
    void (*UpdatePosition)(vec3 position);
};

extern struct ACamera ACamera[1];

#endif
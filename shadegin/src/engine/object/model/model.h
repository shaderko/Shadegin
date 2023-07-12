/**
 * @file model.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>
#include <linmath.h>

typedef struct Model Model;
struct Model
{
    bool is_valid;

    vec3 *verticies;
    int verticies_count;
    unsigned int *indicies;
    int indicies_count;
    vec3 *uvs;
    int uv_count;

    vec3 position;
    vec3 size;
    float rotation;
    vec4 color;
};

struct AModel
{
    Model *(*Init)(vec2 position, vec2 size, float rotation, vec4 color);
    Model *(*Load)(const char *path);
};

extern struct AModel AModel[1];

#endif
/**
 * @file model.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "model.h"

#include <stdio.h>
#include <stdlib.h>

// Init function should be called to load from cache
static Model *Init(vec2 position, vec2 size, float rotation, vec4 color)
{
    Model *model = malloc(sizeof(Model));
    model->is_valid = true;

    memcpy(model->position, position, sizeof(vec2));
    memcpy(model->size, size, sizeof(vec2));
    model->rotation = rotation;
    memcpy(model->color, color, sizeof(vec4));

    return model;
}

static Model *Load(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", path);
        return NULL;
    }

    Model *model = malloc(sizeof(Model));
    model->is_valid = false;
    model->verticies_count = 0;
    model->indicies_count = 0;
    if (model == NULL)
    {
        printf("Failed to allocate memory for model\n");
        fclose(file);
        return NULL;
    }

    char lineHeader[128];
    while (fscanf(file, "%s", lineHeader) != EOF)
    {
        if (strcmp(lineHeader, "v") == 0)
        {
            // Vertices
            model->verticies = realloc(model->verticies, sizeof(vec3) * (model->verticies_count + 1));
            fscanf(file, "%f %f %f", &model->verticies[model->verticies_count][0], &model->verticies[model->verticies_count][1], &model->verticies[model->verticies_count][2]);
            printf("Vertex: %f %f %f\n", model->verticies[model->verticies_count][0], model->verticies[model->verticies_count][1], model->verticies[model->verticies_count][2]);
            model->verticies_count++;
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            // UVs
            // temp_uvs = realloc(temp_uvs, sizeof(vec3) * (uvIndex + 1));
            // fscanf(file, "%f %f %f", &temp_uvs[uvIndex][0], &temp_uvs[uvIndex][1], &temp_uvs[uvIndex][2]);
            // printf("UV: %f %f %f\n", temp_uvs[uvIndex][0], temp_uvs[uvIndex][1], temp_uvs[uvIndex][2]);
            // uvIndex++;
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            // Faces
            unsigned int vertexIndex[3], uvIndex[3];
            // int matches = fscanf(file, "%d/%d/%*d %d/%d/%*d %d/%d/%*d\n",
            //                      &vertexIndex[0], &uvIndex[0],
            //                      &vertexIndex[1], &uvIndex[1],
            //                      &vertexIndex[2], &uvIndex[2]);

            int matches = fscanf(file, "%d %d %dn",
                                 &vertexIndex[0],
                                 &vertexIndex[1],
                                 &vertexIndex[2]);

            model->indicies = realloc(model->indicies, sizeof(unsigned int) * ((model->indicies_count * 3) + 3));
            for (int i = 0; i < 3; i++)
            {
                model->indicies[(model->indicies_count * 3) + i] = vertexIndex[i] - 1; // Indices are 1-based in .obj
            }
            model->indicies_count++;

            if (matches != 3)
            {
                printf("File can't be read by our simple parser\n");
                fclose(file);
                return NULL;
            }

            printf("Face: %d %d %d\n", vertexIndex[0], vertexIndex[1], vertexIndex[2]);

            // // Assign vertices and uvs based on the face indices
            // model->uvs = realloc(model->uvs, sizeof(vec3) * (model->uv_count + 3));

            // for (int i = 0; i < 3; i++)
            // {
            //     memcpy(&model->uvs[model->uv_count + i], &temp_uvs[uvIndex[i] - 1], sizeof(vec3)); // Indices are 1-based in .obj
            // }

            // model->uv_count += 3;
        }
    }

    fclose(file);

    model->is_valid = true;

    // You may initialize the position, size, rotation, and color in here, or outside this function.
    memcpy(model->position, (vec3){0, 0, 0}, sizeof(vec3)); // TODO:
    memcpy(model->size, (vec3){1, 1, 1}, sizeof(vec3));     // TODO:
    memcpy(model->color, (vec4){1, 1, 1, 1}, sizeof(vec4)); // TODO:
    model->rotation = 0;

    puts("Model loaded successfully");

    return model;
}

struct AModel AModel[1] = {{
    Init,
    Load,
}};
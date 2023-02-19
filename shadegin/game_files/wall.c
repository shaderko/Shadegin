#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "wall.h"
#include "../src/engine/global.h"

Wall create_wall(float x, float y, float width, float height) {
    Wall w = {{x + width / 2, y + height / 2}, {width, height}};
    return w;
}

/* 
 * walls: takes all the walls to make into segments
 * 
 * size: is the length of the walls in elements
 * 
 * returns: pointer to array of arrays of vector 2 elements which specify segments
*/
vec2* segments(Wall* walls, int size) {
    vec2* wall_segments = malloc((size + 1) * sizeof(vec2) * 2 * 4);

    for (int i = 0; i < size; i++) {
        Wall wall = walls[i];

        vec2 left_bottom = {wall.position[0] - wall.size[0] / 2, wall.position[1] - wall.size[1] / 2};
        vec2 right_top = {wall.position[0] + wall.size[0] / 2, wall.position[1] + wall.size[1] / 2};
        vec2 right_bottom = {wall.position[0] + wall.size[0] / 2, wall.position[1] - wall.size[1] / 2};
        vec2 left_top = {wall.position[0] - wall.size[0] / 2, wall.position[1] + wall.size[1] / 2};

        vec2* points_array[] = {left_top, right_top, right_bottom, left_bottom};

        for (int x = 0; x < 4; x++) {
            vec2* first_segment = points_array[x];
            vec2* second_segment = points_array[(x + 1) % 4];

            memcpy(wall_segments + (i * 4 * 2) + (x * 2), first_segment, sizeof(vec2));
            memcpy(wall_segments + (i * 4 * 2) + ((x * 2) + 1), second_segment, sizeof(vec2));
        }
    }

    // add segments of the end of screen
    vec2 left_top = {0, global.render.height};
    vec2 right_top = {global.render.width, global.render.height};
    vec2 right_bottom = {global.render.width, 0};
    vec2 left_bottom = {0, 0};

    vec2* points_array[] = {left_top, right_top, right_bottom, left_bottom};

    for (int x = 0; x < 4; x++) {
        vec2* first_segment = points_array[x];
        vec2* second_segment = points_array[(x + 1) % 4];

        memcpy(wall_segments + ((size) * 4 * 2) + (x * 2), first_segment, sizeof(vec2));
        memcpy(wall_segments + ((size) * 4 * 2) + ((x * 2) + 1), second_segment, sizeof(vec2));
    }

    return wall_segments;
}

Wall random_wall() {
    float x = ((float)rand()/(float)(RAND_MAX)) * global.render.width;
    float y = ((float)rand()/(float)(RAND_MAX)) * global.render.height;
    Wall w = {{x + 20 / 2, y + 20 / 2}, {20, 20}};
    return w;
}

#include "wall.h"
#include "../../src/engine/render/render.h"

static Wall* walls = NULL;
static size_t size = 0;

void add_wall(vec3 position, vec3 wall_size, vec4 color) {
    walls = realloc(walls, (size + 1) * sizeof(Wall));
    if (!walls) {
        // Handle allocation error TODO:
        return;
    }
    Wall wall = {0};
    position[2] = MIN_DEPTH_VALUE * position[1];
    memcpy(wall.position, position, sizeof(vec3));
    memcpy(wall.size, wall_size, sizeof(vec3));
    memcpy(wall.color, color, sizeof(vec4));
    walls[size] = wall;
    size++;
}

void render_walls() {
    for (int i = 0; i < size; i++) {
        render_quad(walls[i].position, walls[i].size, walls[i].color, false);
        vec3 roof_position = {walls[i].position[0], walls[i].position[1] + walls[i].size[1], walls[i].position[2]};
        vec3 roof_size = {walls[i].size[0], walls[i].size[2], walls[i].size[2]};
        render_quad(roof_position, roof_size, walls[i].color, false);
    }
}

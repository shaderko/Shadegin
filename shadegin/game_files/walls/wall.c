#include "wall.h"
#include "../../src/engine/render/render/render.h"
#include "../../src/engine/util/util.h"

static Wall *walls = NULL;
static size_t size = 0;

void add_wall(vec3 position, vec3 wall_size, vec4 color)
{
    walls = realloc(walls, (size + 1) * sizeof(Wall));
    if (!walls)
    {
        ERROR_EXIT("error allocating memory for wall.\n");
    }
    Wall wall = {0};
    // position[2] = MIN_DEPTH_VALUE * (position[1] - wall_size[1] / 2);
    memcpy(wall.position, position, sizeof(vec3));
    memcpy(wall.size, wall_size, sizeof(vec3));
    // GameObject** array_z = malloc(sizeof(GameObject**) * wall_size[2]);
    for (int z = 0; z < wall_size[2] / 5; z++)
    {
        // GameObject** array_y = malloc(sizeof(GameObject**) * wall_size[1]);
        for (int y = 0; y < wall_size[1] / 5; y++)
        {
            // GameObject** array_x = malloc(sizeof(GameObject*) * wall_size[0]);
            for (int x = 0; x < wall_size[0] / 5; x++)
            {
                // GameObject* object = add_object((vec3){position[0] + x * 5, position[1] + y * 5, position[2] + z * 5}, (vec3){20, 20, 20}, (vec3){0, 0, 0}, 5, false, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){5, 5, 5});
                // array_x[x] = object;
            }
            // array_y[y] = array_x;
        }
        // array_y[y] = array_x;
    }
    // wall.objects = array_y;
    memcpy(wall.color, color, sizeof(vec4));
    walls[size] = wall;
    size++;
}

void render_walls()
{
    for (int i = 0; i < size; i++)
    {
        render_quad(walls[i].position, walls[i].size, walls[i].color, false);
        vec3 roof_position = {walls[i].position[0], walls[i].position[1] + walls[i].size[1] / 2 + walls[i].size[2] / 2, walls[i].position[2]};
        vec3 roof_size = {walls[i].size[0], walls[i].size[2], walls[i].size[2]};
        render_quad(roof_position, roof_size, walls[i].color, false);
    }
}

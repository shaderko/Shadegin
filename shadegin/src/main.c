#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/types.h"
#include "engine/threading/thread_pool.h"

#include "../game_files/point.h"


int main(int argc, char *argv[]) {
    render_init();

    thread_pool_init(8, 50);

    int grid_width = 20;
    int grid_height = 20;
    int grid_offest = 20;

    Point** pointer_array = create_grid(grid_width, grid_height, global.render.width / 2 - (grid_width * grid_offest) / 2, global.render.height - 50, grid_offest);

    bool running = true;
    bool mouse_down = false;
    Point* mouse_point = NULL;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("MOUSE DOWN %i\n", !mouse_down);
                if (mouse_point != NULL) {
                    mouse_point->velocity[0] = 0;
                    mouse_point->velocity[1] = 0;
                }
                mouse_point = NULL;
                mouse_down = !mouse_down;
            default:
                break;
            }
        }

        render_begin();

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseY = global.render.height - mouseY;

        simulate_gravity(pointer_array, grid_width, grid_height);
        if (mouse_down && mouse_point == NULL) {
            for (int y = 0; y < grid_height; y++) {
                for (int x = 0; x < grid_width; x++) {
                    if (mouseX >= pointer_array[y][x].position[0] && mouseX <= pointer_array[y][x].position[0] + 10 && mouseY >= pointer_array[y][x].position[1] && mouseY <= pointer_array[y][x].position[1] + 10) {
                        mouse_point = &pointer_array[y][x];
                    }
                }
            }
        }
        if (mouse_point != NULL) {
            // printf("mouse: %i %i", mouseX, mouseY);
            mouse_point->position[0] = mouseX;
            mouse_point->position[1] = mouseY;
        }

        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                draw_connection(&pointer_array[y][x]);
            }
        }
        // for (int y = 0; y < grid_height; y++) {
        //     for (int x = 0; x < grid_width; x++) {
        //         draw_point(&pointer_array[y][x]);
        //     }
        // }

        render_end();
    }

    thread_pool_cleanup();

    return 0;
}

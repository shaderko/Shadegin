#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/types.h"

#include "../game_files/wall.h"
#include "../game_files/raycast.h"

int main(int argc, char *argv[]) {
    render_init();

    bool running = true;

    int walls_size = 50;
    Wall* walls = malloc(sizeof(Wall) * walls_size);
    for (int i = 0; i < walls_size; i++) {
        walls[i] = random_wall();
    }
    vec2* rays_array = segments(walls, walls_size);
    size_t segments_size = ((walls_size) + 1);


    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }

        render_begin();

        vec2* rays = malloc(0);
        size_t rays_size = 0;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (int i = 0; i < ((walls_size) + 1) * 4; i++) {
            rays = cast_rays(rays, &rays_size, (vec2){mouseX, global.render.height - mouseY}, (vec2){rays_array[i * 2][0], rays_array[i * 2][1]}, rays_array, segments_size);
        }
        sort_rays(rays, rays_size, (vec2){mouseX, global.render.height - mouseY});
        draw_rays(rays, rays_size, mouseX, global.render.height - mouseY);

        for (int i = 0; i < walls_size; i++) {
            render_quad(walls[i].position, walls[i].size, (vec4){1, 1, 1, 1}, false);
        }

        render_end();
    }

    return 0;
}

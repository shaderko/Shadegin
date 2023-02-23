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

    int walls_size = 100;
    Wall* walls = malloc(sizeof(Wall) * walls_size);
    for (int i = 0; i < walls_size; i++) {
        walls[i] = random_wall(10, 10);
    }
    // Wall screen_wall = create_wall(0, 0, global.render.width, global.render.height);
    size_t segments_size = ((walls_size) + 1);
    vec2 *segments_array = segments(walls, walls_size);


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

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        for (int i = 0; i < (walls_size); i++) {
            cast_rays(walls, walls_size, (vec2){mouseX, global.render.height - mouseY}, i, segments_array, segments_size);
        }
        for (int i = 0; i < walls_size; i++) {
            if (walls[i].render) {
                render_quad(walls[i].position, walls[i].size, (vec4){1, 1, 1, 1}, true);
            }
        }
        
        draw_rays(walls, walls_size, mouseX, global.render.height - mouseY);

        render_end();
    }
    
    free(segments_array);

    return 0;
}

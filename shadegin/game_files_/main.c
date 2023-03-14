#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/types.h"
#include "engine/threading/thread_pool.h"

#include "../game_files/wall.h"
#include "../game_files/raycast.h"

int main(int argc, char *argv[]) {
    render_init();
    thread_pool_init(8, 50);

    bool running = true;

    int walls_size = 10;
    Wall* walls = create_random_walls(walls_size, 10, 10);

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
        mouseY = global.render.height - mouseY;

        for (int i = 0; i < (walls_size); i++) {
            CastRaysArguments* args = (CastRaysArguments*)malloc(sizeof(CastRaysArguments));
            args->walls = walls;
            args->walls_size = walls_size;
            args->src_pos_x = mouseX;
            args->src_pos_y = mouseY;
            args->index = i;
            args->segments = segments_array;
            args->size = walls_size + 1;
            Task task = {
                .function = cast_rays,
                .args = args,
                .completed = 0
            };
            add_task(task);
        }
        finished();
        draw_rays(walls, walls_size, mouseX, mouseY);
        draw_walls(walls, walls_size);

        render_end();
    }
    
    free(walls);
    free(segments_array);

    thread_pool_cleanup();

    return 0;
}

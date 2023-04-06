#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/types.h"
#include "engine/threading/thread_pool.h"

#include "../game_files/player.h"
#include "../game_files/walls/wall.h"
#include "engine/game_objects/collider.h"
#include "engine/game_objects/game_object.h"

int main(int argc, char *argv[]) {
    render_init();

    thread_pool_init(8, 5000);

    add_wall((vec3){200, 200}, (vec3){30, 30, 20}, (vec4){1, 1, 1, 1});
    add_wall((vec3){400, 400}, (vec3){100, 100, 10}, (vec4){1, 1, 1, 1});

    add_object((vec3){100, 100}, (vec3){100, 100, 10}, (vec3){0, 0, 0}, 3, false, SQUARE, &(vec3){100, 100, 10});
    add_object((vec3){0, 0}, (vec3){400, 20, 10}, (vec3){0, 0, 0}, 3, true, SQUARE, &(vec3){400, 20, 10});
    add_object((vec3){0, 0}, (vec3){400, 20, 10}, (vec3){0, 0, 0}, 3, true, CIRCLE, &(float){1});

    bool running = true;
    bool mouse_down = false;

    player_init((vec2){0, 0}, .1);

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("MOUSE DOWN %i\n", !mouse_down);
                mouse_down = !mouse_down;
            default:
                break;
            }
        }

        simulate_gravity(-.000001);

        render_begin();

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseY = global.render.height - mouseY;

        render_walls();
        render_game_objects();

        move_player((vec2){mouseX, mouseY});
        render_player();

        render_end();
    }

    thread_pool_cleanup();

    return 0;
}

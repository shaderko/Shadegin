#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/types.h"
#include "engine/threading/thread_pool.h"
#include "engine/render/camera.h"
#include "engine/game_objects/collider.h"
#include "engine/game_objects/game_object.h"

int main(int argc, char *argv[]) {
    render_init();

    thread_pool_init(8, 5000);
    int particle_size = 20;
    for (int i = 0; i < particle_size; i++) {
        add_object((vec3){i * 21, 500, 0}, (vec3){20, 20, 20}, (vec3){0, 0, 0}, 1, false, COLIDER_CIRCLE, RENDERER_CIRCLE, &(float){10});
    }
    GameObject* object1 = add_object((vec3){100, 600, -0.00004}, (vec3){100, 100, 100}, (vec3){0, 0, 0}, 3, false, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){100, 100, 100});
    add_object((vec3){0, 0}, (vec3){100, 100, 100}, (vec3){0, 0, 0}, 3, true, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){400, 20, 10});

    bool running = true;
    bool mouse_down = false;

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

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseY = global.render.height - mouseY;
        // if (mouse_down) {
        camera_update_position((vec3){mouseX, mouseY, 0});
        // mouse_down = false;
        // }
        simulate_gravity(-.00001);

        render_begin();


        // render_begin_pixelated();
        render_game_objects();
        Camera* camera = get_camera();

        // render_end_pixelated();

        render_light((vec3){mouseX + camera->position[0] / 2, mouseY + camera->position[1] / 2, 200});

        render_end();
    }

    thread_pool_cleanup();

    return 0;
}

#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/types.h"
#include "engine/threading/thread_pool.h"
#include "engine/render/camera.h"

// #include "../game_files/player.h"
// #include "../game_files/walls/wall.h"
// #include "engine/game_objects/game_object.h"

int main(int argc, char *argv[])
{
    render_init();

    thread_pool_init(8, 5000);

    // add_wall((vec3){50, 100, 0}, (vec3){20, 20, 20}, (vec4){1, 1, 1, 1});

    // GameObject* object1 = add_object((vec3){20, 150, 0}, (vec3){100, 100, 100}, (vec3){0, 0, 0}, 3, false, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){25, 25, 25});
    // add_object((vec3){0, 0}, (vec3){100, 100, 100}, (vec3){0, 0, 0}, 3, true, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){100, 5, 500});
    // add_object((vec3){25, 0}, (vec3){25, 25, 25}, (vec3){0, 0, 0}, 3, true, COLIDER_CIRCLE, RENDERER_CIRCLE, &(float){1});

    // GameObject *object = AGameObject->Init();
    // GameObject *object1 = AGameObject->InitBox(10, 10, 10);

    bool running = true;
    bool mouse_down = false;

    // player_init((vec2){0, 0}, .1);
    Camera *camera = get_camera();
    // camera->target = get_player();

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
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
        // object1->position[2] = MIN_DEPTH_VALUE * mouseY;
        // if (mouse_down) {
        // camera_follow_target();
        // mouse_down = false;
        // }
        // printf("%f\n", object1->position[2]);

        // simulate_gravity(-.001);

        // move_player((vec2){mouseX + camera->position[0] - global.render.width / 2, mouseY + camera->position[1] - global.render.height / 2});

        render_begin();

        // render_begin_pixelated();

        // object1->renderer->Render(object1->renderer, (vec3){10, 10, 10});
        render_square((vec3){mouseX, mouseY, 0}, (vec3){500, 500, 50}, (vec4){1, 1, 1, 1}, true);
        render_quad((vec3){mouseX, mouseY, 0}, (vec3){500, 500, 50}, (vec4){1, 1, 1, 1}, true);
        // render_game_objects();
        // render_collider(object1);
        // render_player();

        // render_end_pixelated();
        // render_walls();

        render_light((vec3){mouseX, mouseY, 55});

        render_end();
    }

    thread_pool_cleanup();

    return 0;
}

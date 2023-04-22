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
#include "engine/game_objects/game_object.h"
#include "engine/networking/client.h"

int main(int argc, char *argv[])
{
    render_init();

    thread_pool_init(8, 5000);

    // add_wall((vec3){50, 100, 0}, (vec3){20, 20, 20}, (vec4){1, 1, 1, 1});

    // GameObject* object1 = add_object((vec3){20, 150, 0}, (vec3){100, 100, 100}, (vec3){0, 0, 0}, 3, false, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){25, 25, 25});
    // add_object((vec3){0, 0}, (vec3){100, 100, 100}, (vec3){0, 0, 0}, 3, true, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){100, 5, 500});
    // add_object((vec3){25, 0}, (vec3){25, 25, 25}, (vec3){0, 0, 0}, 3, true, COLIDER_CIRCLE, RENDERER_CIRCLE, &(float){1});

    // GameObject *object = AGameObject->Init();
    AGameObject->InitBox(false, 1.0, (vec3){10, 300, 10}, (vec3){50, 50, 50});
    GameObject *object = AGameObject->InitBox(false, 1.0, (vec3){60, 400, 10}, (vec3){50, 50, 50});
    AGameObject->InitBox(true, 1.0, (vec3){0, 0, 0}, (vec3){100, 50, 50});

    object->renderer->position[0] += 50;

    AClient->Init();

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
            case SDL_MOUSEWHEEL:
                printf("mouse wheel\n");
                if (event.wheel.y > 0) // scroll up
                {
                    printf("mouse wheel up\n");
                    camera->distance -= 10;
                    render_update_projection(camera);
                }
                else if (event.wheel.y < 0) // scroll down
                {
                    printf("mouse wheel down\n");
                    camera->distance += 10;
                    render_update_projection(camera);
                }
            default:
                break;
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseY = global.render.height - mouseY;

        // camera_follow_target();

        camera_update_position((vec3){mouseX - global.render.width / 2, mouseY - global.render.height / 2, 0});
        AGameObject->UpdateGameObjects();

        render_begin();
        render_begin_pixelated();

        AGameObject->RenderGameObjects();

        render_end_pixelated();

        render_light((vec3){mouseX, mouseY, 55});

        render_end();
    }

    thread_pool_cleanup();

    return 0;
}

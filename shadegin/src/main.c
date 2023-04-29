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
#include "engine/networking/server.h"
#include "engine/networking/client.h"

int main(int argc, char *argv[])
{
    render_init();

    // thread_pool_init(8, 5000);

    // AGameObject->InitBox(false, 1.0, (vec3){10, 300, 10}, (vec3){50, 50, 50});
    GameObject *object = AGameObject->InitBox(false, 1.0, (vec3){60, 400, 10}, (vec3){50, 50, 50});
    // AGameObject->InitBox(true, 1.0, (vec3){0, 0, 0}, (vec3){100, 50, 50});

    AServer->Init();

    // object->renderer->position[0] += 50;

    Client *client = AClient->Init();

    bool running = true;
    bool mouse_down = false;

    // player_init((vec2){0, 0}, .1);
    Camera *camera = get_camera();
    // camera->target = get_player();

    while (running)
    {
        Uint32 startTime = SDL_GetTicks();

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
                if (mouse_down)
                {
                    AClient->JoinRoom(client, 124);
                }
                else
                {
                    // AClient->SendObject(client, object);
                }
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

        if (mouse_down)
        {
            object->position[0] = mouseX;
            object->position[1] = mouseY;

            AClient->SendObject(client, object);
        }

        // camera_follow_target();

        camera_update_position((vec3){mouseX - global.render.width / 2, mouseY - global.render.height / 2, 0});
        AGameObject->UpdateGameObjects();

        render_begin();
        // render_begin_pixelated();

        AGameObject->RenderGameObjects();

        // render_end_pixelated();

        render_light((vec3){200, 200, 55});

        render_end();

        // FPS and Keep alive functions
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        // Calculate FPS and print to console
        // float fps = 1000.0f / elapsedTime;
        // printf("FPS: %.2f\n", fps);

        // Regulate frame rate so the game doesn't consume computah
        if (elapsedTime < 16)
        {
            SDL_Delay(16 - elapsedTime);
            float fps = 1000.0f / (16 - elapsedTime);
            // printf("FPS: %.2f\n", fps);
        }
    }

    // thread_pool_cleanup();

    return 0;
}

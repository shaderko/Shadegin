#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/types.h"
#include "engine/threading/thread_pool.h"
#include "engine/render/camera.h"

#include "../game_files/player.h"
// #include "../game_files/walls/wall.h"
#include "engine/game_objects/game_object.h"
#include "engine/networking/server.h"
#include "engine/networking/client.h"
#include "engine/game_objects/map/scene.h"

#include <stdlib.h>
#include "../leak_detector_c.h"

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-s") == 0)
        {
            AServer->Init();
            return 0;
        }
    }

    Client *client = AClient->Init();
    if (client == NULL)
    {
        return 0;
    }

    // Scene *scene = AScene->Init(&(vec3){0, 0, 0});
    // AScene->ReadFile(scene, "file");

    render_init();

    bool running = true;
    bool mouse_down = false;

    Camera *camera = ACamera->Get();
    Player *player = APlayer->Init((vec2){0, 0}, 10);
    float movement_x = 0;
    float movement_y = 0;

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
                    AClient->JoinRoom(client, 0);
                }
                mouse_down = !mouse_down;
            case SDL_MOUSEWHEEL:
                printf("mouse wheel\n");
                if (event.wheel.y > 0)
                {
                    printf("mouse wheel up\n");
                    camera->distance -= 10;
                    render_update_projection(camera);
                }
                else if (event.wheel.y < 0)
                {
                    printf("mouse wheel down\n");
                    camera->distance += 10;
                    render_update_projection(camera);
                }
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    AClient->JoinRoom(client, 136773233543873);
                }
                else if (event.key.keysym.sym == SDLK_w)
                {
                    GameObject *object = AGameObject->GetGameObjectByIndex(1);
                    object->position[1] += 100;
                    AClient->SendObject(client, object);
                    movement_y = 10;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    movement_y = -10;
                }
                else if (event.key.keysym.sym == SDLK_a)
                {
                    movement_x = -10;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    movement_x = 10;
                }
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_w)
                {
                    movement_y = 0;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    movement_y = 0;
                }
                else if (event.key.keysym.sym == SDLK_a)
                {
                    movement_x = 0;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    movement_x = 0;
                }
                break;
            default:
                break;
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseY = global.render.height - mouseY;

        if (movement_x != 0 || movement_y != 0)
        {
            APlayer->Move(player, (vec2){movement_x, movement_y});
        }

        ACamera->UpdatePosition((vec3){mouseX - global.render.width / 2 + player->object->position[0], mouseY - global.render.height / 2 + player->object->position[1], 0});
        AGameObject->UpdateGameObjects();

        render_begin();
        // render_begin_pixelated();

        AGameObject->RenderGameObjects();

        // render_end_pixelated();

        render_light((vec3){200, 200, 55});

        render_end();

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;
        // Regulate frame rate so the game doesn't consume computah
        if (elapsedTime < 16)
        {
            SDL_Delay(16 - elapsedTime);
        }
        else
        {
            float fps = 1000.0f / elapsedTime;
            printf("FPS too low %.2f\n", fps);
        }
    }

    return 0;
}

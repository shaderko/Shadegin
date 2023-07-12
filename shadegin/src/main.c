#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/common/global/global.h"
#include "engine/common/types/types.h"
#include "engine/threading/thread_pool/thread_pool.h"
#include "engine/render/camera/camera.h"

#include "../game_files/player.h"
// #include "../game_files/walls/wall.h"

#include "engine/object/game_object/game_object.h"
#include "engine/network/server/server.h"
#include "engine/network/client/client.h"
#include "engine/object/map/scene.h"

#include "engine/common/config/config.h"
#include "engine/input/input.h"

#include "engine/object/model/model.h"

#define SPEED_PLAYER 1

#include <stdlib.h>
// #include "../leak_detector_c.h"

static Client *client = NULL;

static void input_handle(Player *player)
{
    f32 velx = 0;
    f32 vely = 0;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mouseY = global.render.height - mouseY;

    if (global.input.left == KS_PRESSED || global.input.left == KS_HELD)
    {
        velx = -SPEED_PLAYER;
    }
    if (global.input.right == KS_PRESSED || global.input.right == KS_HELD)
    {
        velx = SPEED_PLAYER;
    }
    if (global.input.up == KS_PRESSED || global.input.up == KS_HELD)
    {
        vely = SPEED_PLAYER;
    }
    if (global.input.down == KS_PRESSED || global.input.down == KS_HELD)
    {
        vely = -SPEED_PLAYER;
    }
    if (global.input.jump == KS_PRESSED)
    {
        AClient->JoinRoom(client, 8977600565253564081);
    }
    APlayer->Move(player, (vec2){velx, vely});

    ACamera->FollowTarget(&(vec2){mouseX, mouseY});

    if (velx != 0 || vely != 0)
    {
        AClient->SendObject(client, player->object);
    }
}

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

    // client = AClient->Init();
    // if (client == NULL)
    // {
    //     return 0;
    // }

    // Scene *scene = AScene->Init(&(vec3){0, 0, 0});
    // AScene->ReadFile(scene, "file");

    // Model *model = AModel->Load("/Users/filiplukovic/Documents/projects/shadegin/shadegin/build/cube.obj");
    // Renderer *renderer = ARenderer->InitMesh(model);

    config_init();
    render_init();

    bool running = true;
    bool mouse_down = false;

    Camera *camera = ACamera->Get();
    Player *player = APlayer->Init((vec2){0, 0}, 10);
    camera->target = player->object;

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
                if (mouse_down)
                {
                    AClient->JoinRoom(client, 0);
                }
                mouse_down = !mouse_down;
            case SDL_MOUSEWHEEL:
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
            default:
                break;
            }
        }

        input_update();
        input_handle(player);

        AGameObject->UpdateGameObjects();

        render_begin();
        render_begin_pixelated();

        // renderer->Render(renderer, (vec3){0, 0, 0});

        AGameObject->RenderGameObjects();

        render_end_pixelated();

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

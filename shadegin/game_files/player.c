/**
 * @file player.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-05-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "player.h"
#include "../src/engine/render/render/render.h"

static Player **players = NULL;
static size_t players_size = 0;

Player *Init(vec2 position, float speed)
{
    Player *player = malloc(sizeof(Player));
    if (!player)
    {
        ERROR_EXIT("Failed to allocate memory for player");
    }

    players = realloc(players, sizeof(Player *) * (players_size + 1));
    if (!players)
    {
        ERROR_EXIT("Failed to allocate memory for players");
    }

    // Assign values
    player->speed = 0;
    player->object = AObject.InitBox(true, 1, (vec3){position[0], position[1], 1}, (vec3){5, 5, 5});

    players[players_size] = player;
    players_size++;

    return player;
}

Player *Get()
{
    if (players_size == 0)
    {
        return NULL;
    }
    return players[0];
}

void Move(Player *player, vec2 position)
{
    player->object->velocity[0] = position[0];
    player->object->velocity[1] = position[1];

    player->object->position[0] += player->object->velocity[0];
    player->object->position[1] += player->object->velocity[1];
}

void Render()
{
    for (int i = 0; i < players_size; i++)
    {
        AObject.Render(players[i]->object);
    }
}

struct APlayer APlayer[1] =
    {{
        Init,
        Get,
        Move,
        Render,
    }};
/**
 * @file room.c
 * @author https://github.com/shaderko
 * @brief Room is used as one game (where the actual game runs)
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../util.h"

#include "room.h"
#include "server.h"

static Room *Init(Server *server)
{
    Room *room = malloc(sizeof(Room));
    if (room == NULL)
    {
        ERROR_EXIT("Error allocating space for room\n");
    }

    room->clients = NULL;
    room->clients_size = 0;
    room->room_id = generate_random_id();

    server->rooms = realloc(server->rooms, (server->rooms_size + 1) * sizeof(Room *));
    if (server->rooms == NULL)
    {
        ERROR_EXIT("Error allocating space for server rooms\n");
    }

    server->rooms[server->rooms_size] = room;
    server->rooms_size++;

    return room;
}

/**
 * @brief Get the Room object
 *
 * @param server - server that should be searched for the room id
 * @param room_id - room id
 * @return Room* if room with the given id exists, otherwise NULL
 */
static Room *GetRoom(Server *server, int room_id)
{
    for (int i = 0; i < server->rooms_size; i++)
    {
        if (server->rooms[i]->room_id == room_id)
        {
            return server->rooms[i];
        }
    }

    return NULL;
}

static void RemoveClient(Room *room, ServerClient *client)
{
    for (int i = 0; i < room->clients_size; i++)
    {
        if (room->clients[i] == client)
        {
            if (room->clients_size <= 1)
            {
                room->clients[i] = NULL;
            }
            room->clients[i] = room->clients[room->clients_size - 1];
            room->clients_size--;
        }
    }
}

struct ARoom ARoom[1] = {{Init, GetRoom, RemoveClient}};
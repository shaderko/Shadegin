/**
 * @file server.c
 * @author https://github.com/shaderko
 * @brief Server is only used to accept connections and assign clients to rooms
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "server.h"
#include "room.h"
#include "../util.h"

static void Init()
{
    if (SDLNet_Init() != 0)
    {
        ERROR_EXIT("Error initializing SDL_net: %s\n", SDLNet_GetError());
    }

    Server *server = malloc(sizeof(Server));
    if (server == NULL)
    {
        ERROR_EXIT("Server memory couldn't be allocated!\n");
    }

    if (SDLNet_ResolveHost(&server->ip, NULL, 1234) == -1)
    {
        ERROR_EXIT("Error resolving server host: %s\n", SDLNet_GetError());
    }

    // Opening server
    server->server = SDLNet_UDP_Open(1234);
    if (!server->server)
    {
        ERROR_EXIT("Error opening server socket: %s\n", SDLNet_GetError());
    }

    UDPpacket *packet = SDLNet_AllocPacket(512);
    if (!packet)
    {
        ERROR_EXIT("Error allocating UDP packet: %s\n", SDLNet_GetError());
    }

    printf("Server initialized! Receving data.\n");

    // Main server loop
    while (1)
    {
        AServer->ReceiveData(server, packet);
        SDL_Delay(10); // Add a small delay to reduce CPU usage
    }

    // Close the server socket
    SDLNet_UDP_Close(server->server);
}

static void ReceiveData(Server *server, UDPpacket *packet)
{
    if (SDLNet_UDP_Recv(server->server, packet))
    {
        printf("Received %d bytes from %x:%d\n", packet->len, SDLNet_Read32(&packet->address.host), SDLNet_Read16(&packet->address.port));

        Message message;
        memcpy(&message, packet->data, sizeof(Message));

        // Check if client is in room or wants to join one
        if (message.room_id == NULL) // If room id is NULL it is keep alive message
        {
            // KEEP ALIVE TODO:
            return;
        }

        Room *room = ARoom->GetRoom(server, message.room_id);
        if (message.room_id == 0)
        {
            room = ARoom->Init(server);
        }
        else if (room == NULL)
        {
            // Room doesn't exist send back response error TODO:
            return;
        }

        for (int i = 0; i < room->clients_size; i++)
        {
            if (room->clients[i]->address.host == packet->address.host)
            {
                // Found client, assign action to room thread
            }
        }
    }
}

struct AServer AServer[1] =
    {{Init, ReceiveData}};
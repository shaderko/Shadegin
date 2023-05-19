/**
 * @file client.c
 * @author https://github.com/shaderko
 * @brief Server-Client connection
 * @version 0.1
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../util.h"

#include "client.h"
#include "server.h"

static Client *Init()
{
    if (SDLNet_Init() != 0)
    {
        ERROR_EXIT("Error initializing SDL_net: %s\n", SDLNet_GetError());
    }

    Client *client = malloc(sizeof(Client));
    if (SDLNet_ResolveHost(&client->ip, "127.0.0.1", 1234) == -1)
    {
        free(client);
        ERROR_EXIT("Error resolving server host: %s\n", SDLNet_GetError());
    }

    client->server = SDLNet_UDP_Open(0);
    if (!client->server)
    {
        free(client);
        ERROR_EXIT("Error opening client socket: %s\n", SDLNet_GetError());
    }

    client->id = -1;
    client->room_id = 0; // Change after TODO:
    if (AClient->Connect(client) || AClient->Login(client))
    {
        free(client);
        ERROR_EXIT("Couldn't connect to server.");
    }

    return client;
}

static int Connect(Client *client)
{
    if (client == NULL)
    {
        return 1;
    }

    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message));
    if (packet == NULL)
    {
        return 1;
    }
    Message message = {0, CONNECTION_REQUEST, 0, NULL};

    packet->address.host = client->ip.host;
    packet->address.port = client->ip.port;
    memcpy(packet->data, &message, sizeof(Message));
    packet->len = sizeof(Message);

    while (1)
    {
        puts("Connecting.");
        if (SDLNet_UDP_Send(client->server, -1, packet) == 0)
        {
            printf("Error sending packet\n");
            return 1;
        }
        SDL_Delay(100);
        if (SDLNet_UDP_Recv(client->server, packet) == 1)
        {
            Message *message = malloc(sizeof(Message));
            memcpy(message, packet->data, sizeof(Message));

            client->id = message->client_id;
            printf("client id %d\n", client->id);

            free(message);
            break;
        }
        SDL_Delay(2000);
    }
    puts("Connected!");
    SDLNet_FreePacket(packet);
    return 0;
}

static int Login(Client client)
{
    puts("Logging in.");
    return 0;
}

static void JoinRoom(Client *client, Uint32 room_id)
{
    if (client == NULL)
    {
        return;
    }

    Message message = {client->id, JOIN_ROOM_REQUEST, sizeof(room_id), room_id};
    if (room_id == 0)
    {
        message.type = CREATE_ROOM_REQUEST;
    }
    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message));

    packet->address.host = client->ip.host;
    packet->address.port = client->ip.port;
    memcpy(packet->data, &message, sizeof(Message));
    packet->len = sizeof(Message);

    if (SDLNet_UDP_Send(client->server, -1, packet) == 0)
    {
        printf("Error sending packet\n");
        return;
    }
    printf("Packet sent\n");

    // Wait for response and assign room to client
    while (1)
    {
        if (SDLNet_UDP_Recv(client->server, packet) == 1)
        {
            Message *message = malloc(sizeof(Message));
            memcpy(message, packet->data, sizeof(Message));

            if (message->type != JOIN_ROOM_RESPONSE)
            {
                printf("problem\n");
                free(message);
                break;
            }

            client->id = message->data;
            printf("room id %d\n", client->id);
            free(message);

            break;
        }
        SDL_Delay(10);
    }
    SDLNet_FreePacket(packet);
}

static void SendObject(Client *client, GameObject *object)
{
    // printf("sending game object %d\n", object->id);
    // if (client->room_id <= 0)
    // {
    //     printf("no room id\n");
    //     return;
    // }

    // SerializedDerived derived = AGameObject->Serialize(object);
    // Message message = {client->id, client->room_id, GAME_OBJECT, derived.len, NULL};
    // SerializedGameObject *xd = derived.data;
    // printf("sending : %f, %f, %f\n", xd->position[0], xd->position[1], xd->position[2]);

    // UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message) + derived.len);

    // packet->address.host = client->ip.host;
    // packet->address.port = client->ip.port;
    // memcpy(packet->data, &message, sizeof(Message));
    // memcpy(packet->data + sizeof(Message), derived.data, derived.len);
    // packet->len = sizeof(Message) + derived.len;

    // printf("sending %lu bytes\n", derived.len);

    // if (SDLNet_UDP_Send(client->server, -1, packet) == 0)
    // {
    //     printf("Error sending packet\n");
    //     return;
    // }
    printf("Packet sent 2\n");

    // free(derived.data);
    // SDLNet_FreePacket(packet);
}

struct AClient AClient[1] =
    {{Init, Connect, Login, JoinRoom, SendObject}};
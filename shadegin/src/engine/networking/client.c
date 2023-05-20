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
#include "../types.h"

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
    client->room_id = 0;

    // TODO: use threads to connect and login, so the app doesn't freeze
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

static int Login(Client *client)
{
    puts("Logging in.");
    return 0;
}

static void JoinRoom(Client *client, ull room_id)
{
    // REMOVE AFTER TODO:
    if (client->room_id != 0)
    {
        return;
    }

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

            if (message->type != JOIN_ROOM_RESPONSE && message->type != CREATE_ROOM_RESPONSE)
            {
                puts("Problem creating/joining room");
                free(message);
                break;
            }

            client->room_id = (ull)message->data;
            printf("room id %lld\n", client->room_id);
            free(message);

            break;
        }
        SDL_Delay(10);
    }
    SDLNet_FreePacket(packet);
}

static void ReceiveObject(Client *client)
{
    if (client == NULL || client->room_id <= 0)
    {
        puts("no room id");
        return;
    }

    UDPpacket *packet = SDLNet_AllocPacket(512);
    if (packet == NULL)
    {
        ERROR_EXIT("Error allocating UDP packet: %s\n", SDLNet_GetError());
    }

    // while (1)
    // {
    //     if (SDLNet_UDP_Recv(client->server, packet) == 1)
    //     {
    //         puts("Receiving data.");

    //         Message *message = malloc(sizeof(Message));
    //         memcpy(message, packet->data, sizeof(Message));

    //         if (message->type != DATA_RESPONSE)
    //         {
    //             printf("Problem receiving object %i\n", message->type);
    //             free(message);
    //             SDLNet_FreePacket(packet);
    //             return;
    //         }

    //         SerializedGameObject *object = malloc(sizeof(SerializedGameObject));
    //         memcpy(object, packet->data + sizeof(Message), sizeof(SerializedGameObject));

    //         printf("received : %f, %f, %f\n", object->position[0], object->position[1], object->position[2]);
    //         AGameObject->Deserialize(object, object->collider.derived.data, object->renderer.derived.data);

    //         free(message);
    //         free(object);
    //     }
    //     else
    //     {
    //         puts("No packets received.");
    //     }
    //     SDL_Delay(10);
    // }
    // SDLNet_FreePacket(packet);
    if (SDLNet_UDP_Recv(client->server, packet) <= 0)
    {
        puts("No packets received.");
        SDLNet_FreePacket(packet);
        return;
    }

    puts("Receiving data.");

    Message *message = malloc(sizeof(Message));
    memcpy(message, packet->data, sizeof(Message));

    if (message->type != DATA_RESPONSE)
    {
        puts("Problem receiving object");
        free(message);
        SDLNet_FreePacket(packet);
        return;
    }

    SerializedGameObject *object = malloc(message->length);
    memcpy(object, packet->data + sizeof(Message), message->length);

    printf("received : %f, %f, %f\n", object->position[0], object->position[1], object->position[2]);
    AGameObject->Deserialize(object, object->collider.derived.data, object->renderer.derived.data);

    free(message);
    free(object);
    SDLNet_FreePacket(packet);
}

static void SendObject(Client *client, GameObject *object)
{
    printf("sending game object %lld\n", object->id);
    if (client->room_id <= 0)
    {
        printf("no room id\n");
        return;
    }

    AServer->SendObject(client->server, client->ip, object, client->id);

    // SerializedDerived derived = AGameObject->Serialize(object);
    // Message message = {client->id, DATA_RESPONSE, derived.len, NULL};
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
    // printf("Packet sent 2\n");

    // free(derived.data);
    // SDLNet_FreePacket(packet);
}

struct AClient AClient[1] =
    {{Init, Connect, Login, JoinRoom, ReceiveObject, SendObject}};
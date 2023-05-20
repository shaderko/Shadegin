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
#include "../types.h"

static Server *server = NULL;

static void Init()
{
    if (SDLNet_Init() != 0)
    {
        ERROR_EXIT("Error initializing SDL_net: %s\n", SDLNet_GetError());
    }

    server = malloc(sizeof(Server));
    if (server == NULL)
    {
        free(server);
        ERROR_EXIT("Server memory couldn't be allocated!\n");
    }
    server->clients = NULL;
    server->clients_size = 0;

    server->rooms = NULL;
    server->rooms_size = 0;

    if (SDLNet_ResolveHost(&server->ip, NULL, 1234) == -1)
    {
        free(server);
        ERROR_EXIT("Error resolving server host: %s\n", SDLNet_GetError());
    }

    // Opening server
    server->server = SDLNet_UDP_Open(1234);
    if (!server->server)
    {
        free(server);
        ERROR_EXIT("Error opening server socket: %s\n", SDLNet_GetError());
    }

    UDPpacket *packet = SDLNet_AllocPacket(512);
    if (!packet)
    {
        free(server);
        ERROR_EXIT("Error allocating UDP packet: %s\n", SDLNet_GetError());
    }

    puts("Server initialized! Receving data.");

    // Main server loop
    while (1)
    {
        AServer->ReceiveData(packet);
        SDL_Delay(10); // Add a small delay to reduce CPU usage
    }

    // Close the server socket
    SDLNet_UDP_Close(server->server);
}

static void Response(IPaddress address, int client_id, MessageType type, int size, int *data)
{
    // Packet could be allocated when the server is initialized TODO:
    Message message = {client_id, type, size, data};

    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message));
    packet->address.host = address.host;
    packet->address.port = address.port;
    packet->len = sizeof(Message);

    memcpy(packet->data, &message, sizeof(Message));

    printf("Sending %zu bytes\n", sizeof(message));

    if (SDLNet_UDP_Send(server->server, -1, packet) == 0)
    {
        printf("Error sending packet\n");
        return;
    }
}

static ServerClient *AddClient(IPaddress address)
{
    server->clients = realloc(server->clients, sizeof(ServerClient) * (server->clients_size + 1));
    if (server->clients == NULL)
    {
        // Free the server TODO: low prio
        ERROR_EXIT("Server clients couldn't be allocated!\n");
    }

    ServerClient client = {server->clients_size, address, SDL_GetTicks(), NULL};
    server->clients[server->clients_size] = client;
    server->clients_size++;

    return &server->clients[server->clients_size - 1];
}

static ServerClient *GetClient(int client_id, IPaddress address)
{
    if (client_id > server->clients_size)
    {
        return NULL;
    }

    ServerClient *client = &server->clients[client_id];
    if (client == NULL)
    {
        return NULL;
    }

    if (client->address.host == address.host && client->address.port == address.port)
    {
        return client;
    }

    return NULL;
}

static void ReceiveData(UDPpacket *packet)
{
    int packets = SDLNet_UDP_Recv(server->server, packet);
    if (packets <= 0)
    {
        if (packets == -1)
        {
            ERROR_EXIT("Error receiving data: %s\n", SDLNet_GetError());
        }
        return;
    }

    Message *message = malloc(sizeof(Message));
    memcpy(message, packet->data, sizeof(Message));

    ServerClient *client = NULL;
    Room *room = NULL;

    if (message->type == CONNECTION_REQUEST)
    {
        client = AServer->AddClient(packet->address);
        // Respond with connection accept
        AServer->Response(packet->address, client->id, CONNECTION_RESPONSE, 0, NULL);

        free(message);
        return;
    }
    else if (message->type == LOGIN_REQUEST)
    {
        // LOGIN
        free(message);
        return;
    }

    client = AServer->GetClient(message->client_id, packet->address);
    if (client == NULL)
    {
        // TODO: client has been disconnected, delete from the server clients list
        puts("client disconnected");
        free(message);
        return;
    }

    // Process the type of data sent
    switch (message->type)
    {
    case CREATE_ROOM_REQUEST:
        room = ARoom->Init(server);
        ARoom->JoinClient(room, client);
        printf("client address 2 %d\n", client->address.host);
        if (room == NULL)
        {
            AServer->Response(packet->address, client->id, ERROR_NOTIFICATION, 0, NULL);
            // Respond with failure to create room
        }
        AServer->Response(packet->address, client->id, CREATE_ROOM_RESPONSE, sizeof(ull), room->room_id);
        // Respond with success
        free(message);
        break;

    case JOIN_ROOM_REQUEST:
        room = ARoom->GetRoom(server, message->data);
        if (room == NULL)
        {
            AServer->Response(packet->address, client->id, ERROR_NOTIFICATION, 0, NULL);
            break;
        }
        // Add client to room, and add room to client
        ARoom->JoinClient(room, client);
        AServer->Response(packet->address, client->id, JOIN_ROOM_RESPONSE, sizeof(ull), room->room_id);
        free(message);
        break;

    case DATA_RESPONSE:
        if (client->room == NULL)
        {
            break;
        }
        room = client->room;

        message->data = malloc(message->length);
        memcpy(message->data, packet->data + sizeof(Message), message->length);

        SDL_LockMutex(room->queue->mutex);
        room->queue->data[room->queue->tail] = message;
        room->queue->size++;
        room->queue->tail++;

        if (room->queue->tail > room->queue->capacity)
        {
            room->queue->tail = 0;
        }
        SDL_UnlockMutex(room->queue->mutex);

    default:
        break;
    }

    if (room == NULL)
    {
        char error_msg[] = "Room doesn't exist or failed to create.";

        printf("%s\n", error_msg);
        AServer->Response(packet->address, client->id, ERROR_NOTIFICATION, sizeof(error_msg), &error_msg);

        return;
    }

    // AServer->Response(packet->address, client->id, CONNECTION_RESPONSE, 0, NULL);
}

static void SendObject(UDPsocket socket, IPaddress address, GameObject *object, int client_id)
{
    SerializedDerived derived = AGameObject->Serialize(object);
    Message message = {client_id, DATA_RESPONSE, derived.len, NULL};
    SerializedGameObject *xd = derived.data;
    printf("sending : %f, %f, %f\n", xd->position[0], xd->position[1], xd->position[2]);

    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message) + derived.len);

    packet->address.host = address.host;
    packet->address.port = address.port;
    memcpy(packet->data, &message, sizeof(Message));
    memcpy(packet->data + sizeof(Message), derived.data, derived.len);
    packet->len = sizeof(Message) + derived.len;

    printf("sending %lu bytes\n", derived.len + sizeof(Message));

    if (SDLNet_UDP_Send(server->server, -1, packet) == 0)
    {
        printf("Error sending packet\n");
        return;
    }

    free(derived.data);
    SDLNet_FreePacket(packet);
}

static Server *GetServer()
{
    return server;
}

struct AServer AServer[1] =
    {{Init, Response, AddClient, GetClient, ReceiveData, SendObject, GetServer}};
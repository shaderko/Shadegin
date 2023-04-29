/**
 * @file server.c
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "server.h"
#include "room.h"
#include "../game_objects/game_object.h"

static void Init()
{
    Server *server = malloc(sizeof(Server));
    if (server == NULL)
    {
        // TODO: error
        printf("Server memory couldn't be allocated!\n");
        return;
    }

    if (SDLNet_ResolveHost(&server->ip, NULL, 1234) == -1)
    {
        printf("Error resolving server host: %s\n", SDLNet_GetError());
        return;
    }

    // Opening server
    server->server = SDLNet_UDP_Open(1234);
    if (!server->server)
    {
        printf("Error opening server socket: %s\n", SDLNet_GetError());
        return;
    }

    // Create socket set for all the connected clients
    server->connections = NULL;
    server->socket_connections = SDLNet_AllocSocketSet(10);
    if (server->socket_connections == NULL)
    {
        printf("Error allocating memory for socket set\n");
    }

    UDPpacket *packet = SDLNet_AllocPacket(512);
    if (!packet)
    {
        printf("Error allocating UDP packet: %s\n", SDLNet_GetError());
        return;
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

static void DeleteSocket(Server *server, ServerClient *client, int i)
{
    SDLNet_UDP_DelSocket(server->socket_connections, client->socket);
    SDLNet_UDP_Close(client->socket);

    server->connections[i] = server->connections[server->connections_size - 1];
    server->connections_size--;
    printf("Client %d disconnected!\n", i);
}

static Message *GetMessage(ServerClient *client)
{
    int start = SDL_GetTicks();

    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message));
    if (!packet)
    {
        printf("SDLNet_AllocPacket failed: %s\n", SDLNet_GetError());
        return NULL;
    }

    Message *message = (Message *)malloc(sizeof(Message));
    int bytesReceived = 0;

    if (SDLNet_UDP_Recv(client->socket, packet) == 1)
    {
        printf("Message bytes received %d\n", packet->len);
        message = (Message *)packet->data;
    }
    return message;
}

static void ReceiveData(Server *server, UDPpacket *packet)
{
    if (SDLNet_UDP_Recv(server->server, packet))
    {
        printf("Received %d bytes from %x:%d\n", packet->len, SDLNet_Read32(&packet->address.host), SDLNet_Read16(&packet->address.port));

        Message message;
        memcpy(&message, packet->data, sizeof(Message));

        // Extract the game object data from the packet's data field
        GameObject gmo;
        memcpy(&gmo, packet->data + sizeof(Message), sizeof(GameObject));
        AGameObject->Deserialize(&gmo);

        // Process the received data here
        // You may want to store the client's address for future communication

        // Send a response to the client
        // const char *response = "Message received";
        // memcpy(packet->data, response, strlen(response) + 1);
        // packet->len = strlen(response) + 1;

        // if (SDLNet_UDP_Send(serverSocket, -1, packet) == 0)
        // {
        //     printf("Error sending response: %s\n", SDLNet_GetError());
        // }
    }
    //     if (sockets_ready <= 0)
    //     {
    //         return;
    //     }

    //     ServerClient *client = server->connections[i];
    //     if (!SDLNet_SocketReady(client->socket))
    //     {
    //         continue;
    //     }
    //     sockets_ready--;

    //     // Receive message
    //     printf("receiving message from %d\n", i);
    //     Message *message = GetMessage(client);
    //     if (message == NULL)
    //     {
    //         AServer->DeleteSocket(server, client, i);
    //         continue;
    //     }

    //     // Receive data
    //     message->data = (int *)malloc(message->length);
    //     int bytesReceived = 0;
    //     while (bytesReceived < message->length)
    //     {
    //         bytesReceived += SDLNet_UDP_Recv(client->socket, message->data + bytesReceived, message->length);
    //         printf("Data bytes received %d\n", bytesReceived);
    //     }

    //     int *room_id = NULL;
    //     GameObject *gmo = NULL;

    //     printf("checking message type\n");
    //     switch (message->type)
    //     {
    //     case ROOM_ID:
    //         room_id = message->data;

    //         printf("Received room id %d bytes: %i\n", bytesReceived, *room_id);
    //         printf("joining room %d\n", *room_id);

    //         Room *room = ARoom->GetRoom(server, *room_id);
    //         if (room == NULL)
    //         {
    //             room = ARoom->Init(server);
    //         }

    //         if (client->room != NULL)
    //         {
    //             ARoom->RemoveClient(room, client);
    //         }

    //         room->clients = realloc(room->clients, (room->clients_size + 1) * sizeof(ServerClient *));
    //         room->clients[room->clients_size] = client;
    //         room->clients_size++;

    //         client->room = room;

    //         printf("joined room %d, %d\n", room->room_id, client->room->room_id);

    //         break;
    //     case GAME_OBJECT:
    //         gmo = (GameObject *)message->data;
    //         AGameObject->Deserialize(gmo);
    //         printf("Received gmo %d bytes: %i\n", bytesReceived, gmo->id);
    //         break;
    //     default:
    //         continue;
    //     }

    //     // Process received data
    //     printf("Received %d bytes\n", bytesReceived);
}

struct AServer AServer[1] =
    {{Init, DeleteSocket, ReceiveData}};
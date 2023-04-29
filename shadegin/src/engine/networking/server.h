/**
 * @file server.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-04-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <SDL2/SDL_net.h>
#include "room.h"

typedef enum MessageType MessageType;
enum MessageType
{
    GAME_OBJECT,
    ROOM_ID,
};

typedef struct Message Message;
struct Message
{
    MessageType type;
    int length;
    int *data;
};

typedef struct ServerClient ServerClient;
struct ServerClient
{
    IPaddress address;
    Room *room;
};

typedef struct Server Server;
struct Server
{
    IPaddress ip;
    UDPsocket server;
    SDLNet_SocketSet socket_connections;
    ServerClient **connections;
    int connections_size;
    Room **rooms;
    int rooms_size;
};

struct AServer
{
    void (*Init)();
    // void (*CheckConnection)(Server *server);
    void (*DeleteSocket)(Server *server, ServerClient *client, int i);
    void (*ReceiveData)(Server *server, UDPpacket *packet);
};

extern struct AServer AServer[1];

#endif
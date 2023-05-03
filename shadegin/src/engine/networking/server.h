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
    NONE,
    ERROR,
    GAME_OBJECT,
    ROOM_ID,
};

typedef struct Message Message;
struct Message
{
    int client_id;
    int room_id;
    MessageType type;
    int length;
    int *data;
};

typedef struct ServerClient ServerClient;
struct ServerClient
{
    int client_id;
    IPaddress address;
    Uint32 last_sent;
    Room *room;
};

typedef struct Server Server;
struct Server
{
    IPaddress ip;
    UDPsocket server;
    ServerClient *clients;
    int clients_size;
    Room **rooms;
    int rooms_size;
};

struct AServer
{
    void (*Init)();
    Server *(*GetServer)();
    void (*Response)(IPaddress address, int room_id, MessageType type, int size, int *data);
    ServerClient *(*GetClient)(int client_id, IPaddress address);
    void (*ReceiveData)(UDPpacket *packet);
};

extern struct AServer AServer[1];

#endif
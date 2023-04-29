/**
 * @file client.h
 * @author https://github.com/shaderko
 * @brief
 * @version 0.1
 * @date 2023-04-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <SDL2/SDL_net.h>
#include "room.h"
#include "../game_objects/game_object.h"

typedef struct Client Client;
struct Client
{
    IPaddress ip;
    UDPsocket server;
    Room *room;
};

struct AClient
{
    Client *(*Init)();
    Client *(*InitFromSocket)(UDPsocket client_socket);
    // void (*KeepAlive)(Client *client);
    void (*JoinRoom)(Client *client, int room_id);
    void (*SendObject)(Client *client, GameObject *object);
};

extern struct AClient AClient[1];

#endif
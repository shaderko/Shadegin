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
#include "../types.h"
#include "../game_objects/game_object.h"

typedef struct Client Client;
struct Client
{
    int id;
    ull room_id;
    IPaddress ip;
    UDPsocket server;
};

struct AClient
{
    /**
     * Initializes a client, connects and logs in to server
     */
    Client *(*Init)();
    int (*Connect)(Client *client);
    int (*Login)(Client *client);
    void (*JoinRoom)(Client *client, ull room_id);
    void (*SendObject)(Client *client, GameObject *object);
};

extern struct AClient AClient[1];

#endif
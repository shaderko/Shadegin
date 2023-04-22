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

#include <SDL2/SDL_net.h>

#ifndef CLIENT_H
#define CLIENT_H

typedef struct Client Client;
struct Client
{
    IPaddress ip;
    TCPsocket server;
};

struct AClient
{
    Client *(*Init)();
};

extern struct AClient AClient[1];

#endif
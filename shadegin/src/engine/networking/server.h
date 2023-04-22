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

#include <SDL2/SDL_net.h>

#ifndef SERVER_H
#define SERVER_H

typedef struct Server Server;
struct Server
{
    IPaddress ip;
    TCPsocket server;
};

struct AServer
{
    Server *(*Init)();
    void (*Loop)(Server *server);
};

extern struct AServer AServer[1];

#endif
/**
 * @file room.h
 * @author https://github.com/shaderko
 * @brief Room is used as one game (where the actual game runs)
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ROOM_H
#define ROOM_H

#include "SDL2/SDL_net.h"

typedef struct ServerClient ServerClient;

/**
 * Definition for loop include
 */
typedef struct Server Server;

typedef struct Room
{
    /**
     * All connected clients to the current game room (lobby)
     */
    ServerClient **clients;
    int clients_size;

    /**
     * Uniquely generated id for every room
     */
    int room_id;
} Room;

struct ARoom
{
    /**
     * Create a room and add it to server list of games
     */
    Room *(*Init)(Server *server);

    /**
     * Get room with room id
     */
    Room *(*GetRoom)(Server *server, int room_id);

    void (*RemoveClient)(Room *room, ServerClient *client);
};

extern struct ARoom ARoom[1];

#endif
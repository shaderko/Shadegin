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

#include <stdbool.h>
#include "SDL2/SDL_net.h"

typedef struct ServerClient ServerClient;
typedef struct Message Message;

/**
 * Definition for loop include
 */
typedef struct Server Server;

/**
 * TODO: rewrite using same concept of queue as in threadpool
 */
typedef struct RoomQueue RoomQueue;
struct RoomQueue
{
    Message **data;
    int tail;
    int size;
    int capacity;
    SDL_mutex *mutex;
};

typedef struct Room
{
    Server *server;
    RoomQueue *queue;

    /**
     * All connected clients to the current game room (lobby)
     */
    ServerClient **clients;
    int clients_size;

    bool is_active;
    SDL_Thread *thread;

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

    void (*ProcessData)(Room *room);
    void (*SendData)(Room *room);

    void (*DeleteRoom)(Room *room);

    int (*RoomGame)(void *data);

    /**
     * Get room with room id
     */
    Room *(*GetRoom)(Server *server, int room_id);

    void (*JoinClient)(Room *room, ServerClient *client);
    void (*RemoveClient)(Room *room, ServerClient *client);
};

extern struct ARoom ARoom[1];

#endif
/**
 * @file server.h
 * @author https://github.com/shaderko
 * @brief Server-Client connection
 * @version 0.1
 * @date 2023-04-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <SDL2/SDL_net.h>
#include "../game_objects/game_object.h"

// definition for loop include
typedef struct Room Room;

typedef enum MessageType MessageType;
enum MessageType
{
    CONNECTION_REQUEST,
    CONNECTION_RESPONSE,
    LOGIN_REQUEST,
    LOGIN_RESPONSE,
    JOIN_ROOM_REQUEST,
    JOIN_ROOM_RESPONSE,
    CREATE_ROOM_REQUEST,
    CREATE_ROOM_RESPONSE,
    DATA_REQUEST,
    DATA_RESPONSE,
    CLIENT_COMMAND,
    SERVER_COMMAND,
    CLIENT_NOTIFICATION,
    SERVER_NOTIFICATION,
    ERROR_NOTIFICATION,
    DISCONNECTION_REQUEST,
    DISCONNECTION_RESPONSE
};

typedef struct Message Message;
struct Message
{
    int client_id;
    MessageType type;

    /**
     * data
     */
    int length;
    int *data;
};

typedef struct ServerClient ServerClient;
struct ServerClient
{
    int id;
    IPaddress address;

    /**
     * Last time data was sent to client or received from client
     */
    Uint32 last_sent;

    /**
     * Room the client is connected to, this is only here so the server doesn't have to loop through all rooms to find the client
     */
    Room *room;
};

typedef struct Server Server;
struct Server
{
    IPaddress ip;

    /**
     * Socket to send and receive data
     */
    UDPsocket server;

    /**
     * All connected clients to the server
     */
    ServerClient *clients;
    int clients_size;

    /**
     * All rooms created in the server
     */
    Room **rooms;
    int rooms_size;
};

struct AServer
{
    /**
     * Initializes a server
     */
    void (*Init)();

    /**
     * Respond to client request
     */
    void (*Response)(IPaddress address, int client_id, MessageType type, int size, int *data);

    /**
     * Add client to server
     */
    ServerClient *(*AddClient)(IPaddress address);

    /**
     * Get client from server
     */
    ServerClient *(*GetClient)(int client_id, IPaddress address);

    /**
     * Receive data from client
     */
    void (*ReceiveData)(UDPpacket *packet);

    /**
     * Send object to client
     */
    void (*SendObject)(UDPsocket socket, IPaddress address, GameObject *object, int client_id);

    /**
     * Get the Server object
     */
    Server *(*GetServer)();
};

extern struct AServer AServer[1];

#endif
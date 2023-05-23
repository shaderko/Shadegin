/**
 * @file room.c
 * @author https://github.com/shaderko
 * @brief Room is used as one game (where the actual game runs)
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdbool.h>
#include <linmath.h>

#include "../util.h"
#include "room.h"
#include "server.h"
#include "../game_objects/map/scene.h"
#include "../game_objects/game_object.h"

static Room *Init(Server *server)
{
    printf("Creating room\n");

    Room *room = malloc(sizeof(Room));
    if (room == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for room!\n");
    }
    room->server = server;

    room->is_active = true;
    room->clients = NULL;
    room->clients_size = 0;
    room->room_id = generate_random_id();

    // Create queue
    room->queue = malloc(sizeof(RoomQueue));
    if (room->queue == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for room %lld queue!\n", room->room_id);
    }
    room->queue->mutex = SDL_CreateMutex();
    if (room->queue->mutex == NULL)
    {
        ERROR_EXIT("Error failed creating queue mutex! %s\n", SDL_GetError());
    }
    room->queue->data = malloc(sizeof(Message *) * 256);
    if (room->queue->data == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for room %lld queue data!\n", room->room_id);
    }
    room->queue->tail = 0;
    room->queue->size = 0;
    room->queue->capacity = 256;

    server->rooms = realloc(server->rooms, (server->rooms_size + 1) * sizeof(Room *));
    if (server->rooms == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for server rooms, room %lld!\n", room->room_id);
    }
    server->rooms[server->rooms_size] = room;
    server->rooms_size++;

    room->thread = SDL_CreateThread(ARoom->RoomGame, "Thread", room);

    printf("Room created with id %lld\n", room->room_id);

    return room;
}

static void RoomGame(Room *room)
{
    printf("Loading map for room %lld\n", room->room_id);

    room->scene = AScene->Init(&((vec3){0, 0, 0}));
    AScene->ReadFile(room->scene, "file");

    printf("Map loaded, starting main loop\n");

    while (room->is_active)
    {
        Uint32 startTime = SDL_GetTicks();

        // Receive updates
        ARoom->ProcessData(room);

        // Do all the game stuff
        AScene->Update(room->scene);

        // Send game objects
        ARoom->SendData(room);

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;
        if (elapsedTime < 16)
        {
            SDL_Delay(16 - elapsedTime);
        }
        else
        {
            float fps = 1000.0f / elapsedTime;
            printf("Room %lld is clogged, fps: %.2f\n", room->room_id, fps);
        }
    }

    ARoom->DeleteRoom(room);
    SDL_DetachThread(room->thread);
}

static void ProcessData(Room *room)
{
    if (room == NULL)
    {
        ERROR_EXIT("Error processing data.\n");
    }

    SDL_LockMutex(room->queue->mutex);
    while (room->queue->size > 0)
    {
        int index = (room->queue->tail - room->queue->size) % room->queue->capacity;
        if (index < 0)
        {
            index += room->queue->capacity;
        }

        Message *message = room->queue->data[index];

        SerializedGameObject *object = NULL;
        int *collider;
        int *renderer;

        SerializedGameObject data;
        memcpy(&data, message->data, sizeof(SerializedGameObject));
        int *ad_data = malloc(data.collider.derived.len + data.renderer.derived.len);
        memcpy(ad_data, message->data + sizeof(SerializedGameObject), data.collider.derived.len + data.renderer.derived.len);
        AScene->Add(room->scene, AGameObject->Deserialize(&data, ad_data, ad_data + data.collider.derived.len, room->scene));

        free(ad_data);
        free(message->data);
        free(message);

        room->queue->size--;
        room->queue->data[index] = NULL;
    }
    SDL_UnlockMutex(room->queue->mutex);
}

static void SendData(Room *room)
{
    Server *server = AServer->GetServer();

    if (room->clients_size <= 0)
    {
        return;
    }

    for (int i = 0; i < room->scene->objects_size; i++)
    {
        GameObject *object = room->scene->objects[i];
        if (!object)
        {
            ERROR_EXIT("Object is NULL!\n");
        }
        for (int j = 0; j < room->clients_size; j++)
        {
            ServerClient *client = room->clients[j];
            if (!client)
            {
                ERROR_EXIT("Client is NULL!\n");
            }

            AServer->SendObject(server->server, client->address, object, client->id);
        }
    }

    printf("Sent %d of objects to %d clients\n", room->scene->objects_size, room->clients_size);
}

/**
 * @brief Deletes room from server's list of rooms and sets all clients room assigned to this room, room to NULL
 * then removes the room and its memory
 *
 * @param room - room to delete
 */
static void DeleteRoom(Room *room)
{
    printf("deleting room\n");
    for (int i = 0; i < room->clients_size; i++)
    {
        room->clients[i]->room = NULL;
    }

    for (int i = 0; i < room->server->rooms_size; i++)
    {
        if (room->server->rooms[i]->room_id == room->room_id)
        {
            room->server->rooms_size--;
            if (room->server->rooms_size <= 0)
            {
                break;
            }
            room->server->rooms[i] = room->server->rooms[room->server->rooms_size];
            printf("Room deleted room %lld\n", room->room_id);
            break;
        }
    }
    free(room);
}

/**
 * @brief Get the Room object from server
 *
 * @param server - server that should be searched for the room id
 * @param room_id - room id
 * @return Room* if room with the given id exists, otherwise NULL
 */
static Room *GetRoom(Server *server, ull room_id)
{
    for (int i = 0; i < server->rooms_size; i++)
    {
        if (server->rooms[i]->room_id == room_id)
        {
            return server->rooms[i];
        }
    }

    return NULL;
}

/**
 * @brief Add the client to a room
 *
 * @param room
 * @param client
 */
static void JoinClient(Room *room, ServerClient *client)
{
    for (int i = 0; i < room->clients_size; i++)
    {
        if (room->clients[i] == client)
        {
            return;
        }
    }

    room->clients = realloc(room->clients, sizeof(ServerClient *) * (room->clients_size + 1));
    if (room->clients == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for room %lld clients!\n", room->room_id);
    }

    room->clients[room->clients_size] = client;
    room->clients_size++;

    client->room = room;
    puts("Client joined room");
}

/**
 * @brief Removes the client from a room
 *
 * @param room
 * @param client
 */
static void RemoveClient(Room *room, ServerClient *client)
{
    for (int i = 0; i < room->clients_size; i++)
    {
        if (room->clients[i] == client)
        {
            if (room->clients_size <= 1)
            {
                room->clients[i] = NULL;
            }
            room->clients[i] = room->clients[room->clients_size - 1];
            room->clients_size--;
        }
    }
    free(client);
    puts("Client removed from room");
}

struct ARoom ARoom[1] = {{Init,
                          RoomGame,
                          ProcessData,
                          SendData,
                          DeleteRoom,
                          GetRoom,
                          JoinClient,
                          RemoveClient}};
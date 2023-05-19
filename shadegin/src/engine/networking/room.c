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

/**
 * @brief Main game loop for server
 *
 * @param room
 */
static int RoomGame(void *data)
{
    Room *room = (Room *)data;

    printf("Loading map for room %d\n", room->room_id);
    SDL_Delay(2000);
    Scene *scene = AScene->Init(&((vec3){0, 0, 0}));
    // Load map
    // GameObject *object = AGameObject->InitBox(false, 1, (vec3){100, 400, 0}, (vec3){100, 100, 100});
    // GameObject *object1 = AGameObject->InitBox(true, 1, (vec3){100, 100, 0}, (vec3){300, 100, 100});
    // GameObject *object2 = AGameObject->InitBox(false, 1, (vec3){100, 100, 0}, (vec3){100, 100, 100});
    // AScene->Add(scene, object);
    // AScene->Add(scene, object1);
    // AScene->Add(scene, object2);
    // AScene->WriteToFile(scene, "file");
    AScene->ReadFile(scene, "file");
    // for (int i = 0; i < scene->objects_size; i++)
    // {
    //     printf("%i\n", scene->objects[i]->id);
    // }

    printf("Map loaded, starting main loop\n");

    while (room->is_active)
    {
        Uint32 startTime = SDL_GetTicks();

        // Receive updates
        ARoom->ProcessData(room);

        // Do all the game stuff
        AGameObject->UpdateGameObjects();

        // Send game objects
        ARoom->SendData(room);

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;
        if (elapsedTime < 16)
        {
            SDL_Delay(16 - elapsedTime);
            // float fps = 1000.0f / (16 - elapsedTime);
            // printf("Room %d fps: %.2f\n", room->room_id, fps);
        }
        else
        {
            float fps = 1000.0f / elapsedTime;
            printf("Room %d is clogged, fps: %.2f\n", room->room_id, fps);
        }
    }

    free(data);
    ARoom->DeleteRoom(room);
    SDL_DetachThread(room->thread);

    return 0;
}

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
        ERROR_EXIT("Couldn't allocate memory for room %d queue!\n", room->room_id);
    }
    room->queue->mutex = SDL_CreateMutex();
    if (room->queue->mutex == NULL)
    {
        ERROR_EXIT("Error failed creating queue mutex! %s\n", SDL_GetError());
    }
    room->queue->data = malloc(sizeof(Message *) * 256);
    if (room->queue->data == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for room %d queue data!\n", room->room_id);
    }
    room->queue->tail = 0;
    room->queue->size = 0;
    room->queue->capacity = 256;

    server->rooms = realloc(server->rooms, (server->rooms_size + 1) * sizeof(Room *));
    if (server->rooms == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for server rooms, room %d!\n", room->room_id);
    }
    server->rooms[server->rooms_size] = room;
    server->rooms_size++;

    Room *thread_data = malloc(sizeof(Room *));
    if (thread_data == NULL)
    {
        ERROR_EXIT("Couldn't allocate memory for thread data, room %d!\n", room->room_id);
    }
    thread_data = room;
    room->thread = SDL_CreateThread(RoomGame, "Thread", thread_data);

    printf("Room created with id %d\n", room->room_id);

    return room;
}

static void ProcessData(Room *room)
{
    if (room == NULL)
    {
        ERROR_EXIT("Error processing data for room %d\n", room->room_id);
    }
    SDL_LockMutex(room->queue->mutex);

    int index = (room->queue->tail - room->queue->size) % room->queue->capacity;
    if (index < 0)
    {
        index += room->queue->capacity;
    }

    while (room->queue->size > 0)
    {
        // Message *message = room->queue->data[index];

        // SerializedGameObject *object = NULL;
        // int *collider;
        // int *renderer;
        // switch (message->type)
        // {
        // case NONE:
        //     printf("none\n");
        //     break;
        // case GAME_OBJECT:
        //     printf("updating gameobject\n");
        //     object = (SerializedGameObject *)message->data;
        //     collider = message->data + sizeof(SerializedGameObject);
        //     renderer = message->data + sizeof(SerializedGameObject) + object->collider.derived.len;
        //     AGameObject->Deserialize(object, collider, renderer);
        //     break;
        // default:
        //     break;
        // }

        // index++;
        // room->queue->size--;
        // free(message);
    }
    SDL_UnlockMutex(room->queue->mutex);
}

static void SendData(Room *room)
{
    Server *server = AServer->GetServer();
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
            printf("Room deleted room %d\n", room->room_id);
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
static Room *GetRoom(Server *server, Uint32 room_id)
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
        ERROR_EXIT("Couldn't allocate memory for room %d clients!\n", room->room_id);
    }

    room->clients[room->clients_size] = client;
    room->clients_size++;
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
}

struct ARoom ARoom[1] = {{Init,
                          ProcessData,
                          SendData,
                          DeleteRoom,
                          RoomGame,
                          GetRoom,
                          JoinClient,
                          RemoveClient}};
#include "../util.h"

#include "client.h"
#include "server.h"

static Client *Init()
{
    if (SDLNet_Init() != 0)
    {
        ERROR_EXIT("Error initializing SDL_net: %s\n", SDLNet_GetError());
    }

    Client *client = malloc(sizeof(Client));
    if (SDLNet_ResolveHost(&client->ip, "127.0.0.1", 1234) == -1)
    {
        printf("Error resolving server host: %s\n", SDLNet_GetError());
        return NULL;
    }

    client->server = SDLNet_UDP_Open(0);
    if (!client->server)
    {
        printf("Error opening client socket: %s\n", SDLNet_GetError());
        return NULL;
    }

    client->id = -1;
    client->room_id = 0; // Change after TODO:
    if (AClient->CheckConnection(client))
    {
        free(client);
        return NULL;
    }

    return client;
}

static void JoinRoom(Client *client, int room_id)
{
    printf("sending room id\n");
    if (client == NULL)
    {
        return;
    }

    Message message = {client->id, client->room_id, NONE, 0, NULL};
    printf("sending %lu bytes\n", sizeof(message));
    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message));

    packet->address.host = client->ip.host;
    packet->address.port = client->ip.port;
    memcpy(packet->data, &message, sizeof(Message));
    packet->len = sizeof(Message);

    if (SDLNet_UDP_Send(client->server, -1, packet) == 0)
    {
        printf("Error sending packet\n");
        return;
    }
    printf("Packet sent\n");

    while (1)
    {
        if (SDLNet_UDP_Recv(client->server, packet) == 1)
        {
            Message *message = malloc(sizeof(Message));
            memcpy(message, packet->data, sizeof(Message));

            client->room_id = message->room_id;
            printf("room id %d\n", message->room_id);

            break;
        }
        SDL_Delay(10);
    }
    SDLNet_FreePacket(packet);
    // Wait for response and assign room to client
}

static void SendObject(Client *client, GameObject *object)
{
    printf("sending game object %d\n", object->id);
    if (client->room_id <= 0)
    {
        printf("no room id\n");
        return;
    }

    SerializedDerived derived = AGameObject->Serialize(object);
    Message message = {client->id, client->room_id, GAME_OBJECT, derived.len, NULL};
    SerializedGameObject *xd = derived.data;
    printf("sending : %f, %f, %f\n", xd->position[0], xd->position[1], xd->position[2]);

    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message) + derived.len);

    packet->address.host = client->ip.host;
    packet->address.port = client->ip.port;
    memcpy(packet->data, &message, sizeof(Message));
    memcpy(packet->data + sizeof(Message), derived.data, derived.len);
    packet->len = sizeof(Message) + derived.len;

    printf("sending %lu bytes\n", derived.len);

    if (SDLNet_UDP_Send(client->server, -1, packet) == 0)
    {
        printf("Error sending packet\n");
        return;
    }
    printf("Packet sent\n");

    free(derived.data);
    SDLNet_FreePacket(packet);
}

static int CheckConnection(Client *client)
{
    if (client == NULL)
    {
        return 1;
    }

    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message));
    Message message = {client->id, -1, NONE, 0, NULL};

    packet->address.host = client->ip.host;
    packet->address.port = client->ip.port;
    memcpy(packet->data, &message, sizeof(Message));
    packet->len = sizeof(Message);

    while (1)
    {
        printf("Connecting.\n");
        if (SDLNet_UDP_Send(client->server, -1, packet) == 0)
        {
            printf("Error sending packet\n");
            return 1;
        }
        SDL_Delay(100);
        if (SDLNet_UDP_Recv(client->server, packet) == 1)
        {
            Message *message = malloc(sizeof(Message));
            memcpy(message, packet->data, sizeof(Message));

            client->id = message->client_id;
            printf("client id %d\n", client->id);

            free(message);
            break;
        }
        SDL_Delay(2000);
    }
    SDLNet_FreePacket(packet);
    return 0;
}

struct AClient AClient[1] =
    {{Init, CheckConnection, JoinRoom, SendObject}};
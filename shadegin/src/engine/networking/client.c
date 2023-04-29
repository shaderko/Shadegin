#include "client.h"
#include "server.h"

static Client *Init()
{
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

    client->room = NULL;

    return client;
}

static Client *InitFromSocket(UDPsocket client_socket)
{
    Client *client = malloc(sizeof(Client));
    if (client == NULL)
    {
        printf("Error creating client\n");
        return NULL;
    }

    client->ip = *SDLNet_UDP_GetPeerAddress(client_socket, -1);
    client->server = client_socket;
    client->room = NULL;

    return client;
}

// static void KeepAlive(Client *client)
// {
//     const char *KEEP_ALIVE_MESSAGE = "KEEP_ALIVE";
//     int result = SDLNet_UDP_Send(client->server, KEEP_ALIVE_MESSAGE, strlen(KEEP_ALIVE_MESSAGE) + 1);
//     if (result < strlen(KEEP_ALIVE_MESSAGE) + 1)
//     {
//         printf("Error sending keep-alive message: %s\n", SDLNet_GetError());
//     }
// }

static void JoinRoom(Client *client, int room_id)
{
    printf("sending room id\n");
    if (client == NULL)
    {
        return;
    }
    // Message message = {ROOM_ID, sizeof(int), NULL};
    // printf("sending %d bytes\n", sizeof(message));
    // SDLNet_UDP_Send(client->server, &message, sizeof(Message));
    // SDLNet_UDP_Send(client->server, &room_id, sizeof(room_id));
}

static void SendObject(Client *client, GameObject *object)
{
    printf("sending game object %d\n", object->id);
    Message message = {GAME_OBJECT, sizeof(GameObject), NULL};

    UDPpacket *packet = SDLNet_AllocPacket(sizeof(Message) + sizeof(GameObject));

    packet->address.host = client->ip.host;
    packet->address.port = client->ip.port;
    memcpy(packet->data, &message, sizeof(Message));
    memcpy(packet->data + sizeof(Message), object, sizeof(GameObject));
    packet->len = sizeof(Message) + sizeof(GameObject);

    if (SDLNet_UDP_Send(client->server, -1, packet) == 0)
    {
        printf("Error sending packet\n");
        return;
    }
    printf("Packet sent\n");
    // SDLNet_UDP_Send(client->server, object, sizeof(GameObject));
}

// static int *CheckConnections(Client *client)
// {
//     SDLNet_UDP_Send(client->server, );
// }

struct AClient AClient[1] =
    {{Init, InitFromSocket, JoinRoom, SendObject}};
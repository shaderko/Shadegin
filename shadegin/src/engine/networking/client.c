#include "client.h"

static Client *Init()
{
    Client *client = malloc(sizeof(Client));
    if (SDLNet_ResolveHost(&client->ip, "127.0.0.1", 27027) == -1)
    {
        printf("Error resolving server host: %s\n", SDLNet_GetError());
        return NULL;
    }

    client->server = SDLNet_TCP_Open(&client->ip);
    if (!client->server)
    {
        printf("Error opening client socket: %s\n", SDLNet_GetError());
        return NULL;
    }

    // Close the client socket
    // SDLNet_TCP_Close(client->server);
    return client;
}

struct AClient AClient[1] =
    {{Init}};
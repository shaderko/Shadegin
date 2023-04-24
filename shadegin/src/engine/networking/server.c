#include "server.h"

static Server *Init()
{
    Server *server = malloc(sizeof(Server));
    if (server == NULL)
    {
        // TODO: error
        return NULL;
    }

    if (SDLNet_ResolveHost(&server->ip, NULL, 27027) == -1)
    {
        printf("Error resolving server host: %s\n", SDLNet_GetError());
        return NULL;
    }

    server->server = SDLNet_TCP_Open(&server->ip);
    if (!server->server)
    {
        printf("Error opening server socket: %s\n", SDLNet_GetError());
        return NULL;
    }

    return server;
}

static void Loop(Server *server)
{
    // Listen for client connections
    TCPsocket client = NULL;
    while (1)
    {
        client = SDLNet_TCP_Accept(server->server);
        if (client)
        {
            // Handle client connection
            printf("Client connected!\n");

            // Close client connection
            SDLNet_TCP_Close(client);
            printf("Client disconnected.\n");
        }
        SDL_Delay(1000); // Add a small delay to reduce CPU usage
    }

    // Close the server socket
    SDLNet_TCP_Close(server->server);
}

struct AServer AServer[1] =
    {{Init,
      Loop}};
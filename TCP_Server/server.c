#include <stdlib.h>
#include <stdio.h>
#include "extension.h"
#include "socketServer.h"
#define BACKLOG 10 /*Number of allowed connections*/

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./server <#Port_Number> <#Directory_name>\n");
        return 0;
    }
    int port = checkAndGetPort(argv[1]);
    openDirectory(argv[2]);
    int listen_sock, conn_sock;
    int clientPort;
    listen_sock = createSocket();
    bindSocket(listen_sock, port);
    listenSocket(listen_sock, BACKLOG);
    printf("Server started!\n");
    while (1)
    {
        conn_sock = acceptSocket(listen_sock);
        analize(conn_sock, argv[2]);
        close(conn_sock);
    }
    close(listen_sock);
    return 0;
}
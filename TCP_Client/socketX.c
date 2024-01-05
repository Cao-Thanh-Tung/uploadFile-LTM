#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socketX.h"
char recv_data[BUFF_SIZE];
/**
 * @function createSocket: create new socket
 * @return new socket.
 */
int createSocket()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }
    return sock;
}
/**
 * @function connectServer: connect to server socket.
 * @param client_sock: client's socket
 * @param serverIp: server's ip string
 * @param serverPort: server's port
 */
void connectServer(int client_sock, char *serverIp, int serverPort)
{
    struct sockaddr_in server_addr; /*server's address infomation*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(serverPort);
    server_addr.sin_addr.s_addr = inet_addr(serverIp);

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }
}

/**
 * @function receiveMessage: receive message from server
 * @param client_sock: client's socket
 * @return status code
 */
int receiveMessage(int client_sock)
{
    memset(recv_data, '\0', BUFF_SIZE);
    int ret = recv(client_sock, recv_data, BUFF_SIZE, 0);
    if (ret <= 0)
    {
        printf("-ERROR Can not connect to server\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        recv_data[ret] = '\0';
        if (strcmp(recv_data, SUCCESS_CONNECT) == 0)
        {
            printf("+OK Welcome to file server\n");
            return 100;
        }
        else if (strcmp(recv_data, PLEASE_SEND_FILE) == 0)
        {
            printf("+OK please send file\n");
            return 200;
        }
        else if (strcmp(recv_data, RECEIVE_FILE_COMPLETED) == 0)
        {
            printf("+OK successfull upload\n");
            return 201;
        }

        else if (strcmp(recv_data, UNKNOW_COMMAND) == 0)
        {
            printf("-ERROR Unknow command.\n");
            return 300;
        }
        else if (strcmp(recv_data, COMMAND_OVER_LENGTH) == 0)
        {
            printf("-ERROR command length is too long.\n");
            return 301;
        }
    }
}
/**
 * @function sendMessage: send message from client to server
 * @param client_sock: client's socket
 * @param message: message content
 */
void sendMessage(int client_sock, char *message)
{
    int ret = send(client_sock, message, BUFF_SIZE, 0);
    if (ret <= 0)
    {
        printf("-ERROR send():");
        exit(EXIT_FAILURE);
    }
}
/**
 * @function sendFile: send file from client to server
 * @param client_sock: client's socket
 * @param filePath: path of file
 */
void sendFile(int client_sock, char *filePath)
{
    char send_data[BUFF_SIZE];
    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        perror("-ERROR opening file\n");
        exit(EXIT_FAILURE);
    }
    memset(send_data, '\0', BUFF_SIZE);
    size_t bytesRead;
    while ((bytesRead = fread(send_data, 1, BUFF_SIZE, file)) > 0)
    {
        send(client_sock, send_data, BUFF_SIZE, 0);
    }
    fclose(file);
}

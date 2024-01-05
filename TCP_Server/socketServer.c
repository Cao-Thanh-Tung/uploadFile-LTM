#include <stdio.h>
#include <stdlib.h>
#include "socketServer.h"
#include "extension.h"
char recv_data[BUFF_SIZE], send_data[BUFF_SIZE];
char clientIP[INET_ADDRSTRLEN];
char command[BUFF_SIZE];

int clientPort;
/**
 * @function createSocket: create new socket
 * @return return new socket
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
 * @function bindSocket: bind socket to port
 * @param sock: server's socket
 * @param port: server's port
 */
void bindSocket(int sock, int port)
{
    struct sockaddr_in server; /*server's address information*/
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("\nError bind(): ");
        exit(EXIT_FAILURE);
    }
}

/**
 * @function listenSocket: wrap listen function to check error
 * @param sock: server's socket
 * @param backlog: number of connection requests will be queued before further requests are refused
 *
 */
void listenSocket(int sock, int backlog)
{
    if (listen(sock, backlog) == -1)
    {
        perror("\nError listen(): ");
        exit(EXIT_FAILURE);
    }
}

/**
 * @function acceptSocket: Await a connection on socket FD. When a connection arrives, open a new socket to communicate with it
 * @param sock: server's socket
 * @return return the new socket's descriptor.
 */
int acceptSocket(int sock)
{
    int conn_sock;
    struct sockaddr_in client; /*client's address information*/
    int sin_size = sizeof(struct sockaddr_in);
    if ((conn_sock = accept(sock, (struct sockaddr *)&client, &sin_size)) == -1)
    {
        perror("\nError accept(): ");
        exit(EXIT_FAILURE);
    }
    inet_ntop(AF_INET, &client.sin_addr, clientIP, sizeof(clientIP));
    clientPort = ntohs(client.sin_port);
    printf("You got a connection from %s:%d\n", clientIP, clientPort); /* printf client's IP*/
    memset(send_data, '\0', BUFF_SIZE);
    strcpy(send_data, SUCCESS_CONNECT);
    writeLog(clientIP, clientPort, "", "+OK Welcome to file server");
    send(conn_sock, send_data, BUFF_SIZE, 0);
    return conn_sock;
}

/**
 * @function writeFile: write file content from client to a file.
 * @param conn_sock: connect socket's descriptor
 * @param filePath: path of file to write in server
 * @param fileSize: size of file
 */
void writeFile(int conn_sock, char *filePath, int fileSize)
{
    int ret = 0;
    FILE *file = fopen(filePath, "wb"); // "wb" để mở tập tin để ghi dữ liệu nhị phân
    if (file == NULL)
    {
        perror("Khong the mo file");
        return;
    }
    while (fileSize > 0)
    {
        ret = recv(conn_sock, recv_data, BUFF_SIZE, 0);
        if (ret > fileSize)
        {
            ret = fileSize;
        }
        size_t elements_written = fwrite(recv_data, ret, 1, file);
        if (elements_written != 1)
        {
            perror("Loi khi ghi vao file");
            fclose(file);
            return;
        }
        fileSize = fileSize - ret;
    }
    writeLog(clientIP, clientPort, command, "$+OK Successful upload");
    memset(send_data, '\0', BUFF_SIZE);
    strcpy(send_data, RECEIVE_FILE_COMPLETED);
    ret = send(conn_sock, send_data, BUFF_SIZE, 0);
    fclose(file);
}

/**
 * @function sendCode: send status code to client
 * @param conn_sock: connect socket's descriptor
 * @param code: string of status code
 */
void sendCode(int conn_sock, char *code)
{
    memset(send_data, '\0', BUFF_SIZE);
    strcpy(send_data, code);
    send(conn_sock, send_data, BUFF_SIZE, 0);
}

/**
 * @function analize: Receive message from client,
 * parse message based on terminating character to separate commands and call corresponding functions
 * @param conn_sock: sockfd
 * @param folderName: path of folder to store file upload from client.
 */
void analize(int conn_sock, char *folderName)
{
    char filePath[BUFF_SIZE];
    char fileName[BUFF_SIZE - 22];
    int ret = 0;
    memset(command, '\0', BUFF_SIZE);
    int l = -1;
    while (1)
    {
        ret = recv(conn_sock, recv_data, BUFF_SIZE, 0); // blocking
        if (ret <= 0)
        {
            printf("%s:%d disconnect to server\n", clientIP, clientPort);
            break;
        }
        else
        {
            int len = strlen(recv_data);
            for (int i = 0; i < len; i++)
            {
                if (l < BUFF_SIZE)
                {
                    l++;
                }
                if (l < BUFF_SIZE)
                {
                    command[l] = recv_data[i];
                }
                else
                {
                    command[l - 2] = command[l - 1];
                    command[l - 1] = recv_data[i];
                }
                if (l > 1 && command[l - 1] == '\r' && command[l] == '\n')
                {
                    if (l == BUFF_SIZE)
                    {
                        sendCode(conn_sock, COMMAND_OVER_LENGTH);
                        l = -1;
                    }
                    else
                    {
                        command[l - 1] = '\0';
                        printf("%s\n", command);
                        if (strstr(command, "UPLD ") == command)
                        {
                            if (strlen(command + 5) == 0)
                            {
                                sendCode(conn_sock, UNKNOW_COMMAND);
                            }
                            else
                            {
                                memset(fileName, '\0', BUFF_SIZE);
                                int fileSize;
                                if (sscanf(command, "UPLD %4090s %d", fileName, &fileSize) == 2)
                                {
                                    sendCode(conn_sock, PLEASE_SEND_FILE);
                                    sprintf(filePath, "%s/%s", folderName, fileName);
                                    writeFile(conn_sock, filePath, fileSize);
                                }
                                else
                                {
                                    sendCode(conn_sock, UNKNOW_COMMAND);
                                }
                            }
                        }
                        else
                        {
                            sendCode(conn_sock, UNKNOW_COMMAND);
                        }
                        l = -1;
                    }
                }
            }
        }
    }
}
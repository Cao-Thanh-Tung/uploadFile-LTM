#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#define BUFF_SIZE 4096
#define SUCCESS_CONNECT "100"
#define UNKNOW_COMMAND "300"
#define COMMAND_OVER_LENGTH "301"
#define RECEIVE_FILE_COMPLETED "201"
#define PLEASE_SEND_FILE "200"
int createSocket();
void bindSocket(int sock, int port);
void listenSocket(int sock, int backlog);
int acceptSocket(int sock);
void writeFile(int conn_sock, char *filePath, int fileSize);
void analize(int conn_sock, char *folderName);
#endif
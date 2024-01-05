#ifndef SOCKETX_H
#define SOCKETX_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#define BUFF_SIZE 4096
#define SUCCESS_CONNECT "100"
#define UNKNOW_COMMAND "300"
#define COMMAND_OVER_LENGTH "301"
#define RECEIVE_FILE_COMPLETED "201"
#define PLEASE_SEND_FILE "200"
int createSocket();
void connectServer(int client_sock, char *serverIp, int serverPort);
int receiveMessage(int client_sock);
void sendMessage(int client_sock, char *message);
void sendFile(int client_sock, char *filePath);
#endif

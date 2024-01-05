#ifndef EXTENSION_H
#define EXTENSION_H
void validateIPv4(char *input);
int checkAndGetPort(char *portString);
int isDirectoryExists(char *path);
void openDirectory(char *path);
void writeLog(char *clientIP, int clientPort, char *command, char *statusCode);
#endif
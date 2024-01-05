#ifndef HELPER_H
#define HELPER_H
#include <sys/stat.h>
void validateIPv4(char *input);
int checkAndGetPort(char *portString);
long long inputFile(char *filePath, int filePathSize);
long long getFileSize(char *filePath);
void createUPLDMessage(char *filePath, long long fileSize, char *message);
#endif
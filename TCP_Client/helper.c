#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "helper.h"
/**
 *@function validateIPv4: Check the format of ipv4, if it is not an ipv4 string, report an error and stop the program.
 *
 * @param input: ip string that the user enters
 */
void validateIPv4(char *input)
{
    regex_t reegex;
    int status = regcomp(&reegex, "^((25[0-5]|(2[0-4]|1[0-9]|[1-9]|)[0-9]).?){3}(25[0-5]|(2[0-4]|1[0-9]|[1-9]|)[0-9])$", REG_EXTENDED);
    if (status == 0)
    {
        status = regexec(&reegex, input, 0, NULL, 0);
        if (status == 1)
        {
            printf("parameter error: <#IP_Addr> is ipv4 format\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("Compilation error.");
        exit(EXIT_FAILURE);
    }
}
/**
 *@function checkAndGetPort: Check the format of port, if it is not an integer greater than 9, report an error and stop the program
 *
 * @param portString: port string that the user enters
 * @return port: in of type integer
 */
int checkAndGetPort(char *portString)
{
    int port = 0;
    for (int i = 0; i < strlen(portString); i++)
    {
        if (portString[i] < '0' || portString[i] > '9')
        {
            printf("parameter error: <#Port_Number> is positive number\n");
            exit(EXIT_FAILURE);
        }
        port *= 10;
        port += portString[i] - '0';
    }
    return port;
}
/**
 * @function getFileSize: get size of file
 *
 * @param filePath: path of file
 * @return size of file
 *
 */
long long getFileSize(char *filePath)
{
    struct stat file_info;
    if (stat(filePath, &file_info) == 0)
    {
        return (long long)file_info.st_size;
    }
    else
    {
        printf("-Error Can not get file info\n");
        return -1;
    }
}

/**
 * @function inputFile: Tell the user to enter the file path to upload and store that path.
 *
 * @param filePath: pointer point memory area to store file paths
 * @param filePathSize: size of memory of memory area to store file paths
 * @return If the file is found, return the file size. If the file is not found, ask the user to re-enter the file path
 */
long long inputFile(char *filePath, int filePathSize)
{
    long long fileSize;
    while (1)
    {
        memset(filePath, '\0', filePathSize);
        printf("Enter file's path: ");
        fgets(filePath, filePathSize, stdin);
        if (strlen(filePath) <= 1)
        {
            exit(EXIT_FAILURE);
        }
        filePath[strlen(filePath) - 1] = '\0';
        fileSize = getFileSize(filePath);
        if (fileSize != -1)
        {
            return fileSize;
        }
    }
}
/**
 *@function createUPLDMessage: Create message to send UPLD file
 *@param filePath: path of file
 *@param fileSize: size of file
 *@param message: buffer stores the generated UPLD message
 */
void createUPLDMessage(char *filePath, long long fileSize, char *message)
{
    char fileSizeString[16];
    char *lastSlash = strrchr(filePath, '/');
    filePath = (lastSlash != NULL) ? lastSlash + 1 : filePath;
    memset(message, '\0', sizeof(message));
    snprintf(fileSizeString, sizeof(fileSizeString), "%lld", fileSize);
    strcpy(message, "UPLD ");
    strcpy(message + 5, filePath);
    strcpy(message + strlen(message), " ");
    strcpy(message + strlen(message), fileSizeString);
    strcpy(message + strlen(message), "\r\n");
}

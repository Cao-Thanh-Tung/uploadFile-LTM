#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "extension.h"
/**
 * @function isDirectoryExists: check directory is exists or not
 * @param path: path of directory
 * @return if directory is exists return 1, else return 0.
 */
int isDirectoryExists(char *path)
{
    struct stat info;
    return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
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
            printf("parameter error: Port_Number is positive number\n");
            return 0;
        }
        port *= 10;
        port += portString[i] - '0';
    }
    return port;
}
/**
 * @function openDirectory: create directory if directory is not exists
 * @param path: path of directory
 */
void openDirectory(char *path)
{
    if (!isDirectoryExists(path))
    {
        int status = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
        if (status != 0)
        {
            printf("Error: Can not create directory.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @function writeLog: write log to file
 * @param clientIP: client's ip address string
 * @param clientPort: client's port number
 * @param command: client's action
 * @param statusContent: client's action result
 */
void writeLog(char *clientIP, int clientPort, char *command, char *statusContent)
{
    FILE *file = fopen("log_20200569.txt", "a+"); // "wb" để mở tập tin để ghi dữ liệu nhị phân
    if (file == NULL)
    {
        perror("Can not open file!");
        return;
    }
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (strcmp(command, "") == 0)
    {
        fprintf(file, "[%02d/%02d/%d %02d:%02d:%02d]$%s:%d$%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, clientIP, clientPort, statusContent);
    }
    else
    {
        fprintf(file, "[%02d/%02d/%d %02d:%02d:%02d]$%s:%d$%s$%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, clientIP, clientPort, command, statusContent);
    }
    fclose(file);
};

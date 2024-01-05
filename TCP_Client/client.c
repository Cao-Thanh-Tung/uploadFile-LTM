#include <stdio.h>
#include "helper.h"
#include "socketX.h"
int main(int argc, char *argv[])
{
    char send_data[BUFF_SIZE], filePath[BUFF_SIZE - 22];
    if (argc != 3)
    {
        printf("usage: ./client <#IP_Addr> <#Port_Number>\n");
        return 0;
    }
    validateIPv4(argv[1]);
    int port = checkAndGetPort(argv[2]);
    int client_sock;
    int ret;
    // Create socket
    client_sock = createSocket();

    // Request to connect server
    connectServer(client_sock, argv[1], port);

    long long fileSize = inputFile(filePath, sizeof(filePath));
    printf("%s\n", filePath);
    int status = receiveMessage(client_sock);
    if (status == 100)
    {
        while (1)
        {
            createUPLDMessage(filePath, fileSize, send_data);
            sendMessage(client_sock, send_data);
            status = receiveMessage(client_sock);
            if (status == 200)
            {
                sendFile(client_sock, filePath);
                receiveMessage(client_sock);
            }
            fileSize = inputFile(filePath, sizeof(filePath));
        }
    }
    printf("Program end.\n");
    // Step 4: close socket
    close(client_sock);
}
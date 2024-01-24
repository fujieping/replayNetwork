#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>

#define SERVER_PORT 8868
#define IP "192.168.204.128"
#define BUFFER_SIZE 1024

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    struct sockaddr_in Client;
    memset(&Client, 0, sizeof(Client));

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET, IP, (void *)server.sin_addr.s_addr);
    int serverlen = sizeof(server);

    int ret = bind(sockfd, (struct sockaddr*)&server, serverlen);
    if (ret == -1)
    {
        perror("band error");
        exit(-1);
    }

    char buf[BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));

    char serverBuf[BUFFER_SIZE];
    memset(serverBuf, 0, sizeof(serverBuf));

    while (1)
    {
        ret = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);


    }


    return 0;
}


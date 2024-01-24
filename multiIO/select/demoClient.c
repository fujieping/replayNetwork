#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <error.h>
#include <stdlib.h>

#define SERVER_PORT 8080
#define SERVER_IP "192.168.204.128"
#define BUFFER_SIZE 128

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);

    int ret = inet_pton(AF_INET, SERVER_IP, (void *)&(serverAddress.sin_addr.s_addr));
    if (ret == -1)
    {
        perror("inet_pton error");
        exit(-1);
    }

    ret = connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (ret == -1)
    {
        perror("connect error");
        exit(-1);
    }

    char writebuffer[BUFFER_SIZE];
    memset(writebuffer, 0, sizeof(writebuffer));

    char recvBuffer[BUFFER_SIZE];
    memset(recvBuffer, 0, sizeof(recvBuffer));

    int readBytes = 0;
    while (1)
    {
        scanf("%s", writebuffer);

        /* 把字符串的结束符加上*/
        write(sockfd, writebuffer, sizeof(writebuffer) + 1);

        /* 接收数据*/
        readBytes = read(sockfd, recvBuffer, sizeof(recvBuffer) - 1);
        if (readBytes < 0)
        {
            perror("read error");
            exit(-1);
        }
        else if (readBytes == 0)
        {
            
            printf("\n");
            
        }
        else
        {
            printf("recv:%s\n", recvBuffer);
            usleep(500);
        }
    }

    close(sockfd);

    return 0;
}
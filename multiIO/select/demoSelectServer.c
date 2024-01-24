#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>


#define SERVER_PORT 8080
#define MAX_LISTEN 128
#define BUFFER_SIZE 128


/* 用单进程/单线程  实现并发*/
int main()
{
    /* 创建套接字*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    /* 将本地ID和端口绑定*/
    struct sockaddr_in localAddress;
    bzero((void *)&localAddress, sizeof(localAddress));
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(SERVER_PORT);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 绑定*/
    socklen_t localAddresslen = sizeof(localAddress);
    int ret = bind(sockfd, (struct sockaddr*)&localAddress, localAddresslen);
    if (ret == -1)
    {
        perror("bind error");
        exit(-1);
    }

    /* 监听*/
    ret = listen(sockfd, MAX_LISTEN);
    if (ret == -1)
    {
        perror("listen error");
        exit(-1);
    }

    fd_set readSet;
    /* 清空集合*/
    FD_ZERO(&readSet);
    fd_set tmpReadSet;
    bzero(&tmpReadSet, sizeof(tmpReadSet));

    /* 把监听的文件描述符放到集合，让内核帮忙检测*/
    FD_SET(sockfd, &readSet);

    int maxfd = sockfd;

    #if 0/* 设置超时*/
    struct timeval timeValue;
    bzero(&timeValue, sizeof(timeValue));
    timeValue.tv_sec = 5;
    timeValue.tv_usec = 0;
    #endif
    while (1)
    {
        /* 备份读集合*/
        tmpReadSet = readSet;
        ret = select(maxfd + 1, &tmpReadSet, NULL, NULL, NULL);
        if (ret == -1)
        {
            perror("select error");
            break;
        }

        /* 如果sockfd在readSet集合里面*/
        if (FD_ISSET(sockfd, &tmpReadSet))
        {
            int acceptfd = accept(sockfd, NULL, NULL);
            if (acceptfd == -1)
            {
                perror("accept error");
                exit(-1);
            }
            /* 将通信的句柄放到读集合*/
            FD_SET(acceptfd, &readSet);
            /* 更新maxfd的值*/
            maxfd = maxfd < acceptfd ? acceptfd : maxfd;
        }
        /* 程序到这个地方也许有通信*/
        for (int idx = 0; idx <= maxfd; idx++)
        {
            if (idx != sockfd && FD_ISSET(idx, &tmpReadSet))
            {
                char buffer[BUFFER_SIZE];
                bzero(buffer, sizeof(buffer));
                /* 程序到这里一定有通信(老客户)*/
                int readBytes = read(idx, buffer, sizeof(buffer) - 1);
                if (readBytes < 0)
                {
                    perror("read error");
                    /* 将该通信句柄从监听的集合清除*/
                    FD_CLR(idx, &readSet);
                    /* 关闭文件句柄*/
                    close(idx);
                    /* 下一个ready的fd句柄进行通信*/
                    continue;
                }
                else if (readBytes == 0)
                {
                    
                    printf("客服端断开连接..\n");
                    FD_CLR(idx, &readSet);
                    /* 关闭通信句柄*/
                    close(idx);
                }
                else
                {
                    printf("recv:%s\n", buffer);

                    for (int jdx = 3; jdx < readBytes; jdx++)
                    {
                        buffer[jdx] = toupper(buffer[jdx]);
                    }

                    /* */
                    write(idx, buffer, sizeof(buffer));
                    usleep(500);
                }

            }
        }


    }



    /* 关闭文件描述符*/
    close(sockfd);

    return 0 ;
}
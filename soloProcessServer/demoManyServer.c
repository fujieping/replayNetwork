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
#include "threadPool.h"

#define MAX_LISTEN 128
#define SERVER_PORT 8080
#define BUFFER_SIZE 128
#define LOCAL_IPADDRESS "192.168.204.128"

#define MINTHREADS 5
#define MAXTHREADS 10
#define MAXQUEUE 50

void * threadHandle(void *arg)
{
    /* 线程分离*/
    pthread_detach(pthread_self());
    /* 通信句柄*/
    int acceptfd = *(int *)arg;

    /* 通信*/
    /* 接收缓冲区*/
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    /* 发送缓冲区*/
    char replyBuffer[BUFFER_SIZE];
    memset(replyBuffer, 0, sizeof(replyBuffer));

    /* 读取到的字节数*/
    int readBytes = 0;
    while (1)
    {
        readBytes = read(acceptfd, buffer, sizeof(buffer));
        if (readBytes <= 0)
        {
            
            perror("read eror");
            close(acceptfd);
            break;
        }
        else
        {
            /* 读到字符*/
            printf("buffer:%s\n", buffer);
            if (strncmp(buffer, "123456", strlen("123456")) == 0)
            {
                strncpy(replyBuffer,"一起加油123456", sizeof(replyBuffer) - 1);
                sleep(1);
                write(acceptfd, replyBuffer, sizeof(replyBuffer));
            }
            else if(strncmp(buffer, "778899", strlen("778899")) == 0)
            {
                strncpy(replyBuffer,"一起加油778899", sizeof(replyBuffer) - 1);
                sleep(1);
                write(acceptfd, replyBuffer, sizeof(replyBuffer));
            }

        }
    }
    /* 线程退出*/
    pthread_exit(NULL);

}


int main()
{
    /* 初始化线程池*/
    // threadpool_t pool;
    // threadPoolInit(&pool, MINTHREADS, MAXTHREADS, MAXQUEUE);

    /* 信号注册*/
    // signal(SIGINT, sigHander);

    /* 创建socket套接字*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }
    /* 设置端口复用*/
    int enableOpt = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&enableOpt, sizeof(enableOpt));
    if (ret == -1)
    {
        perror("setsockopt error");
        exit(-1);
    }

    /* 绑定*/
    struct sockaddr_in localAddress;
    /* 清除脏数据*/
    memset(&localAddress, 0, sizeof(localAddress));

    /* 地址族*/
    localAddress.sin_family = AF_INET;
    /* 端口需要转成大端*/
    localAddress.sin_port = htons(SERVER_PORT);
    #if 1
    /* INADDR_ANY = 0x00000000 */
    localAddress.sin_addr.s_addr = INADDR_ANY;
    #else/* 不是全0就需要转换*/
    /* ip地址需要转换为大端*/
    inet_pton(AF_INET, LOCAL_IPADDRESS, &(localAddress.sin_addr.s_addr));
    #endif
    int localAddresslen = sizeof(localAddress);
    ret = bind(sockfd, (struct sockaddr *)&localAddress, localAddresslen);
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
    /* 客户端的信息*/
    struct sockaddr_in clientAddress;
    memset(&clientAddress, 0 , sizeof(clientAddress));

    int acceptfd = 0;
    /* 循环接收客户端的请求*/
    while (1)
    {
        socklen_t clientAddressLen = 0;
        acceptfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (acceptfd == -1)
        {
            perror("accept error");
            exit(-1);
        }

#if 1/* 这种情况，每来一个客户就开辟一个线程，*/
        pthread_t tid;
        /* 开一个线程去服务acceptfd*/
        ret = pthread_create(&tid, NULL, threadHandle, (void *)&acceptfd);
        if (ret != 0)
        {
            perror("pthread_create error");
            exit(-1);
        }
#else
        threadPoolAddTask(&pool, threadHandle, (void *)&acceptfd);

#endif

    }
    // threadPoolDestroy(&pool);
    /* 关闭文件描述符*/
    close(sockfd);


    return 0;
}
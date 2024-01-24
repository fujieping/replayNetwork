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
#include <json-c/json.h>
#include <json-c/json_object.h>


#define SERVER_PORT 8868
#define IP "192.168.204.128"
#define BUFFER_SIZE 1024

int main()
{
    struct json_object *jsonObj = json_object_new_object();
    if (jsonObj == -1)
    {
        perror("json_object_new_object error");
        exit(-1);
    }

    struct json_object * wayValue = json_object_new_int64(32);
    if (wayValue == NULL)
    {
        perror("json_object_new_int64 error");
        exit(-1);
    }

    json_object_object_add(jsonObj, "way", wayValue);


    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    char buf[BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));

    char clientBuf[BUFFER_SIZE];
    memset(clientBuf, 0, sizeof(clientBuf));

    int ret = 0;

    while (1)
    {
        ret = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);


    }
}
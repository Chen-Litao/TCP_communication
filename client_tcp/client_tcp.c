/* TCP客户端 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define ERRLOG(errmsg) do{\
                           perror(errmsg);\
                           exit(1);\
                        }while(1)
 
 
int main(int argc, char const* argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s [ip] [ port]\n", argv[0]);
        exit(1);
    }
 
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
        ERRLOG("socket fail");
 
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2]));
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        ERRLOG("connect fail-1");
    
    char buf[1480] = " ";
    while(1)
    {
        fgets(buf, 1480, stdin);
        buf[strlen(buf) - 1] = '\0';
        if (send(sockfd, buf, sizeof(buf), 0) < 0)
            ERRLOG("send fail");
        if (strncmp(buf, "quit", 4) == 0)
            //exit(0);
            break;
        memset(buf, 0, sizeof(buf));
        if (recv(sockfd, buf, sizeof(buf), 0) < 0)
            ERRLOG("recv fail");
        printf("recv from server: %s\n", buf);
    }
    close(sockfd);
    return 0;
}
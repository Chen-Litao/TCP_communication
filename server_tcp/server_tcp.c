/* TCP服务端 */
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
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
        ERRLOG("socket fail");
    //绑定服务器地址信息
    struct sockaddr_in serveradr;
    serveradr.sin_family = AF_INET;
    serveradr.sin_port = htons(atoi(argv[2]));
    serveradr.sin_addr.s_addr = inet_addr(argv[1]);
    if (bind(sockfd, (struct sockaddr*)&serveradr, sizeof(serveradr)) < 0)
        ERRLOG("bind fail");
    //监听
    if (listen(sockfd, 5) < 0)
        ERRLOG("listen fail");
    //阻塞等待客户端连接
    struct sockaddr_in clientaddr;
    socklen_t addr_len = sizeof(clientaddr);
    int acceptfd = accept(sockfd, (struct sockaddr*)&clientaddr, &addr_len);
    if (acceptfd < 0)
        ERRLOG("accept fail");
    printf("client inform:<ip:%s> <port:%d>\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    //收发信息
    char buf[1480] = {0};
    ssize_t bytes;
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        if((bytes = recv(acceptfd, buf, sizeof(buf), 0)) < 0)
            ERRLOG("recv fail");
        else if (bytes == 0)
        {
            printf("the client quited\n");
            exit(1);
        }
        if (strncmp(buf, "quit", 4) == 0)
            break;
        printf("recv data: %s\n", buf);
        strcat(buf, "---recv!");
        if (send(acceptfd, buf, sizeof(buf), 0) < 0)
            ERRLOG("send fail");
    }
    close(acceptfd);
    close(sockfd);
    return 0;
}
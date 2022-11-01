#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <sys/types.h>
#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h> //htons inet_addr
#include <unistd.h> //close
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const * argv[])
{
     if(argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(1);
    }

    
    //第一步：创建套接字
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("fail to sock");
        exit(1);
    }

    //第二步：填充广播信息结构体
    struct sockaddr_in broadcastaddr; //广播信息结构体
    broadcastaddr.sin_family = AF_INET;
    broadcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcastaddr.sin_port = htons(atoi(argv[2]));

    //第三步：将套接字与广播信息结构体绑定
    socklen_t addrlen = sizeof(broadcastaddr);
    if(bind(sockfd, (struct sockaddr *)&broadcastaddr, addrlen) == -1)
    {
        perror("fail to bind");
        exit(1);
    }

    //第四步：进行通信
    char buf[128] ="";
    struct sockaddr_in sendaddr;

    while(1)
    {
        if(recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&sendaddr, &addrlen) == -1)
        {
            perror("fail to recvfrom");
            exit(1);
        }

        printf("[%s - %d]: %s\n", inet_ntoa(sendaddr.sin_addr), ntohs(sendaddr.sin_port), buf);
    }

    return 0;
}
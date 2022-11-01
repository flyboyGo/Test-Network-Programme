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

//广播的发送者案例
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

    //第二步：设置允许发送广播权限
    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1)
    {
        perror("fail to setsockopt");
        exit(1);
    }

    //第三步：填充广播信息结构体
    struct sockaddr_in broadcastaddr; //广播信息结构体
    broadcastaddr.sin_family = AF_INET;
    broadcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcastaddr.sin_port = htons(atoi(argv[2]));

    //第四步：进行通信
    socklen_t addrlen = sizeof(broadcastaddr);
    char buf[128] ="";
    while(1)
    {
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';

        if(sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&broadcastaddr, addrlen) == -1)
        {
            perror("fail to sendto");
            exit(1);
        }
    }

    return 0;
}
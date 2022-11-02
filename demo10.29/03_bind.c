/*

   由于服务器是被动的，客户端是主动的，所以一般先运行服务器，后运行客户端，所以服务
   器需要固定自己的信息（ip地址和端口号），这样客户端才可以找到服务器并与之通信，但
   是客户端一般不需要bind绑定，因为系统会自动给客户端分配ip地址和端口号

函数：
    int bind(int sockfd,const struct sockaddr *myaddr，socklen_t addrlen);
功能：
    将本地协议地址与 sockfd 绑定
参数：
     sockfd： 文件描述符，socket的返回值
     myaddr： 指向特定协议的地址结构指针(网络信息结构体 通用结构体 一般不用)
     addrlen：该地址结构的长度
返回值：
      成功：返回 0   失败：-1
*/

#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <sys/types.h>
#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h> //htons inet_addr
#include <unistd.h> //close
#include <string.h>

//bind实例,一般服务器都需要执行bind函数

int main(int argc, char const *argv[])
{
     // ./a.out 192.168.1.2 8080
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

    //第二步：将服务器的网络信息结构体绑定且进行填充
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    //第三步：将网络信息结构体与套接字绑定
    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("fail to bind");
        exit(1);
    }

    return 0;
}
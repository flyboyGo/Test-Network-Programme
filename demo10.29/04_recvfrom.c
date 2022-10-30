/*
函数：
    ssize_t recvfrom(int sockfd, void *buf,size_t nbytes, int flags, 
                    struct sockaddr *from, socklen_t *addrlen);
功能：
     接收 UDP 数据，并将源地址信息保存在 from 指向的结构中
参数：
    sockfd: 套接字
    buf：接收数据缓冲区
    nbytes:接收数据缓冲区的大小
    flags： 套接字标志(常为 0)
            0 阻塞    MSG_DONTWAIT 非阻塞
    from： 源地址结构体指针，用来保存数据的来源(源的网络信息结构体 自动填充，定义变量传参即可)
    addrlen: from 所指内容的长度
注意：
     通过 from 和 addrlen 参数存放数据来源信息
     from 和 addrlen 可以为 NULL, 表示不保存数据来源
返回值：
      成功:接收到的字符数  失败: -1
*/

#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <sys/types.h>
#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h> //htons inet_addr
#include <unistd.h> //close
#include <string.h>

#define N 128
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
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); // 192.168.1.3
    serveraddr.sin_port = htons(atoi(argv[2])); //9999

    //第三步：将网络信息结构体与套接字绑定
    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("fail to bind");
        exit(1);
    }

    //第四步：接送数据
    char buf[N] = "";
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    while(1)
    {
        if(recvfrom(sockfd, buf, N, 0, (struct sockaddr *)&clientaddr, &addrlen) == -1)
        {
               perror("fail to recvfrom");
               exit(1);
        }

        //打印数据
        //打印客户端的ip地址和端口号
        printf("ip:%s  port:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        //打印数据
        printf("from client : %s\n", buf);
        //清空存储区
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}
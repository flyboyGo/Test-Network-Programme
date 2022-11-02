/*
头文件：
      #include <sys/socket.h>
函数：
    int socket(int family, int type, int protocol);
功能：
    创建一个用于网络通信的 socket 套接字（描述符）
参数：
    family:  通信域，协议族
           AF_UNIX   本地通信
           AF_INET   ipv4网络协议
           AF_INET6  ipv6网络协议
           AF_PACKET 底层接口
    type:套接字的类型
             SOCK_STREAM 流式套接字（tcp）
             SOCK_DGRAM 数据报套接字（udp）
             SOCK_RAW 原始套接字（用于链路层）
    protocol:协议类别(0、IPPROTO_TCP、IPPROTO_UDP 等
             附加协议，如果不需要，则设置为0
返回值：
      成功：套接字(文件描述符)
      失败：‐1
特点：
     创建套接字时，系统不会分配端口
     创建的套接字默认属性是主动的，即主动发起服务的请求;当作为服务器时，往往需要修改为被动的
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    //使用scoket函数创建套接字
    //创建一个用于UDP网络编程的套接字
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("fail to scoket");
        exit(1);
    }

    printf("sockfd = %d\n", sockfd);

    return 0;
}
/*
头文件：#include <sys/types.h>   #include <sys/socket.h>
函数：
  ssize_t sendto(int sockfd，const void *buf, size_t nbytes, 
                 int flags, const struct sockaddr *to, socklen_t addrlen);
功能：
     向 to 结构体指针中指定的 ip，发送 UDP 数据
参数：
     sockfd：套接字
     buf： 发送数据缓冲区
     nbytes: 发送数据缓冲区的大小

     flags：一般为 0
     to：指向目的主机地址结构体的指针
     addrlen：to 所指向内容的长度
返回值：
      成功:发送数据的字符数     失败: -1

注意：
    通过 to 和 addrlen 确定目的地址     可以发送 0 长度的 UDP 数据包
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
int main(int argc, char const *argv[])
{
    // ./a.out 192.168.1.2 8080
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(1);
    }

    //第一步：使用scoket函数创建套接字
    //创建一个用于UDP网络编程的套接字
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("fail to scoket");
        exit(1);
    }

    printf("sockfd = %d\n", sockfd);

    //第二部：填充服务器网络信息结构体 sockaddr_in
    struct sockaddr_in serveraddr;
    socklen_t addrlen =  sizeof(serveraddr);

    serveraddr.sin_family = AF_INET; //协议族， AF_INET：ipv4网络协议
    //serveraddr.sin_addr.s_addr = inet_addr("192.168.1.2");  //ip地址
    //serveraddr.sin_port = htons(8080); //端口号

    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);  //获取屏幕输入的第二个参数
    serveraddr.sin_port = htons(atoi(argv[2])); // 获取屏幕输入的第三个参数  将输入的端口号 字符串 转换为 数值类型

    //第三步：发送数据
    char buf[N] = "";
    while(1)
    {
        fgets(buf, N, stdin);
        buf[strlen(buf) - 1] = '\0'; //把字符串中的\n转换为\0
        if(sendto(sockfd, buf, N, 0, (struct sockaddr *)&serveraddr, addrlen) == -1)
        {
            perror("fail to sendto");
            exit(1);
        }
    }

    //第四步：关闭套接字文件描述符
    close(sockfd);

    return 0;
}
#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <sys/types.h>
#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h> //htons inet_addr
#include <unistd.h> //close
#include <string.h>

#define N 128

//UDP客户端的实现
int main(int argc, char const *argv[])
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

#if 0
    //第二步：客户端自己指定自己的ip地址和端口号，一般不需要指定，系统自动分配
    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = inet_addr(argv[1]); // 客户端的地址
    clientaddr.sin_port = htons(atoi(argv[2])); //客户端的端口号

    if(bind(sockfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) == -1)
    {
        perror("client fail to bind");
        exit(1);
    }
#endif

    //第三步：填充服务器的网络信息结构体
    //inet_addr:将点分十进制字符串ip地址转化为整形数据
    //htons:将主机字节序转化为网络字节序
    //atoi:将数字型字符串转化为整形数据
    struct sockaddr_in serveraddr; //服务器的网络信息结构体
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); 
    serveraddr.sin_port = htons(atoi(argv[2])); 


    //第四步：进行通信
    char buf[N] = "";
    char text[N] = "";
    socklen_t addrlen = sizeof(struct sockaddr_in);

    while(1)
    {
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';

        if(sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
        {
            perror("server fail to sendto");
            exit(1);
        }

        if(recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&serveraddr, &addrlen) == -1)
        {
            perror("server fail to recvfrom");
            exit(1);
        }

        printf("from server: %s\n", text);
        memset(text, 0, sizeof(text));
    }

    //第五步：关闭文件描述符(套接字)
    close(sockfd);

    return 0;
}
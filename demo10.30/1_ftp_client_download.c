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

void do_download(int sockfd, struct sockaddr_in serveraddr)
{
    char filename[128] = "";
    printf("请输入要下载的文件名:");
    scanf("%s", filename);

    //给服务器发送消息 , 告知服务器执行下载操作
    unsigned char text[1024] = "";
    int text_len;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int fd, flags = 0, num = 0;
    size_t bytes;

    //构建 给服务器发送的tftp指令并发送给服务器， 例如01data.txt0octet0
    text_len = sprintf(text, "%c%c%s%c%s%c", 0, 1, filename, 0, "octet", 0);
    if(sendto(sockfd, text, text_len, 0, (struct sockaddr *)&serveraddr, addrlen) == -1)
    {
        perror("fail to sendto");
        exit(1);
    }

    //接收服务器端发送过来的数据，并解析、分析
    while(1)
    {
        if((bytes = recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&serveraddr, &addrlen)) == -1)
        {
            perror("fail to recvfrom");
            exit(1);
        }

        //判断操作码执行相对应的处理
        if(text[1] == 5)
        {
            printf("error : %s\n", text+4);
            return;
        }
        else if(text[1] == 3)
        {
            if(flags == 0)
            {
                //创建文件
                if((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
                {
                    perror("fail to open");
                    exit(1);
                }

                flags = 1;
            }

            //对比编号和接收的数据大小并将文件内容写入文件
            if((num + 1 == ntohs(*(unsigned short *)(text + 2))) && (bytes == 516))
            {
                num = ntohs(*(unsigned short *)(text + 2));
                if(write(fd, text + 4, bytes - 4) < 0)
                {
                    perror("fail to write");
                    exit(1);
                }

                //当文件写入完毕后,给服务发送ACK
                text[1] = 4;

                if(sendto(sockfd, text, 4, 0, (struct sockaddr *)&serveraddr, addrlen) == -1)
                {
                    perror("fail to sendto");
                    exit(1);
                }
            }
            else if((num + 1 == ntohs(*(unsigned short *)(text + 2))) && (bytes < 516))
            {
                if(write(fd, text + 4, bytes - 4) < 0)
                {
                    perror("fail to write");
                    exit(1);
                }

                text[1] = 4;
                if(sendto(sockfd, text, 4, 0, (struct sockaddr *)&serveraddr, addrlen) == -1)
                {
                    perror("fail to sendto");
                    exit(1);
                }

                printf("文件下载完毕\n");
                return;
            }

        }
    }
    
}


int main(int argc, char const * argv[])
{
    if(argc < 2)
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
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); //tftp服务器端的IP地址 192.168.1.2
    serveraddr.sin_port = htons(69);

    //第三步：下载函数
    do_download(sockfd, serveraddr);

    return 0;
}
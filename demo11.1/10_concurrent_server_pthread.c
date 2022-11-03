//tcp服务器的实现
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

#define N 128
#define ERR_LOG(errmsg) do{\
                            perror(errmsg);\
                            exit(1);\
                        }while(0)

typedef struct
{
     struct sockaddr_in addr;
     int acceptfd;
}MSG;


void * pthread_fun(void *arg)
{
    char buf[N] = "";
    ssize_t bytes;
    MSG msg = *(MSG *)arg;
    while(1)
    {
        if((bytes = recv(msg.acceptfd, buf, N, 0)) < 0)
        {
            ERR_LOG("fail to recv");
        }
        else if(bytes == 0)
        {
            printf("The client quited\n");
            pthread_exit(NULL);
        }

        if(strncmp(buf, "quit", 4) == 0)
        {
            pthread_exit(NULL);
        }

        printf("from client: %s\n", buf);

        strcat(buf, " ^_^");
        if(send(msg.acceptfd, buf, N, 0) < 0)
        {
            ERR_LOG("fail to send");
        }
    }
}

//使用多线程实现TCP并发服务器
int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }    

    int sockfd, acceptfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t addrlen = sizeof(serveraddr);

    //第一步：创建套接字
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_LOG("fail to socket");
    }
    
    //将套接字设置为允许重复使用本机地址或者设置为端口复用
    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        ERR_LOG("fail to setsockopt");
    }

    //第二步：填充服务器网络信息结构体
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    //第三步：将套接字与服务器网络信息结构体绑定
    if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        ERR_LOG("fail to bind");
    }

    //第四步：将套接字设置为被动监听状态
    if(listen(sockfd, 5) < 0)
    {
        ERR_LOG("fail to listen");
    }

    while(1)
    {
        //第五步：阻塞等待客户端的连接请求
        if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
        {
             ERR_LOG("fail to accept");
        }

       //打印客户端的信息
       printf("%s -- %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

       //第六步：创建子线程与客户端通信
       MSG msg;
       msg.addr = clientaddr;
       msg.acceptfd = acceptfd;
       pthread_t thread;
       if(pthread_create(&thread, NULL, pthread_fun, &msg) != 0)
       {
           ERR_LOG("fail to pthread_create");
       }

       //设置线程为分离态,自动释放资源
       pthread_detach(thread);
       
    }

    return 0;
}
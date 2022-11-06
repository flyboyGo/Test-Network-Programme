#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char const * argv[])
{

    //使用socket函数创建链路层的原始套接字
    int sockfd;
    if((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
    {
        perror("fail to socket");
        exit(1);
    }

    printf("sockfd = %d\n", sockfd);

    close(sockfd);

    return 0;
}
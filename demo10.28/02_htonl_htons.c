#include <stdio.h>
#include <arpa/inet.h>

/*
头文件：
     #include <arpa/inet.h>
函数：
     uint32_t htonl(uint32_t hostint32);
功能:
     将 32 位主机字节序数据转换成网络字节序数据
参数：
    hostint32：待转换的 32 位主机字节序数据
返回值：
    成功：返回网络字节序的值

头文件：
      #include <arpa/inet.h>
函数：
      uint16_t htons(uint16_t hostint16);
功能：
      将 16 位主机字节序数据转换成网络字节序数据
参数：
       uint16_t：unsigned short int
       hostint16：待转换的 16 位主机字节序数据
返回值：
        成功：返回网络字节序的值




头文件：
      #include <arpa/inet.h>
函数：
      uint32_t ntohl(uint32_t netint32);
功能：
      将 32 位网络字节序数据转换成主机字节序数据
参数：
     uint32_t： unsigned int
     netint32：待转换的 32 位网络字节序数据
返回值：
      成功：返回主机字节序的值

头文件：
      #include <arpa/inet.h>
函数：
      uint16_t ntohs(uint16_t netint16);
功能：
      将 16 位网络字节序数据转换成主机字节序数据
参数：
      uint16_t： unsigned short int
      netint16：待转换的 16 位网络字节序数据
返回值：
      成功：返回主机字节序的值
*/

int main(int argc, char const *argv[])
{

    int a = 0x12345678;
    short b = 0x1234;

    printf("%#x\n", htonl(a));
    printf("%#x\n", htons(b));

    printf("%#x\n", ntohl(htonl(a)));
    printf("%#x\n", ntohs(htons(b)));

    return 0;
}
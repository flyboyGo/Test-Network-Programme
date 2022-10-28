/*
头文件：
      #include <arpa/inet.h>
函数：
      int inet_pton(int family,const char *strptr, void *addrptr);
功能：
      将点分十进制数串转换成 32 位无符号整数
参数：
      family 协议族 :
             AF_INET  IPV4网络协议
             AF_INET6 IPV6网络协议
      strptr 点分十进制数串
      addrptr 32 位无符号整数的地址
返回值：
      成功返回 1 、 失败返回 其它
*/
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
      char ip_str[] = "192.168.3.103";
      unsigned int ip_int = 0;
      unsigned char *ip_p = NULL;

      inet_pton(AF_INET, ip_str, &ip_int);
      printf("in_uint = %d\n", ip_int);

      ip_p = (char *)&ip_int;
      printf("in_uint = %d.%d.%d.%d\n", *ip_p, *(ip_p+1), *(ip_p+2), *(ip_p+3));

      return 0;
}
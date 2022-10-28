/*
头文件：
      #include <arpa/inet.h>
函数：
     const char *inet_ntop(int family, const void *addrptr,char *strptr, size_t len);
功能：
     将 32 位无符号整数转换成点分十进制数串
参数：
     family 协议族 : 
             AF_INET  IPV4网络协议
             AF_INET6 IPV6网络协议
     addrptr 32 位无符号整数
     strptr 点分十进制数串
     len strptr 缓存区长度
len 的宏定义:
         #define INET_ADDRSTRLEN 16  //for ipv4
         #define INET6_ADDRSTRLEN 46 //for ipv6
返回值：
       成功:则返回字符串的首地址   失败:返回 NULL
*/

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
     unsigned char ip_int[] = {192, 168, 3, 103};
     char ip_str[16] = "";

     inet_ntop(AF_INET, &ip_int, ip_str, 16);

     printf("ip_s = %s\n", ip_str);

      return 0;
}
#include <stdio.h>

union un
{
    int a;
    char b;
};

union un2
{
    int a;
    short b;
};



int main(int argc, char const *argv[])
{
#if 0
    union un myun;
    myun.a = 0x12345678;

    printf("a = %#x\n", myun.a);
    printf("b = %#x\n", myun.b);

    if(myun.b == 0x78)
    {
        printf("小端存储!\n");
    }
    else
    {
        printf("大端存储!\n");
    }
#endif

#if 1
    union un2 myun;
    myun.a = 1;

    printf("a = %d\n", myun.a);
    printf("b = %d\n", myun.b);

    if(myun.b == 1)
    {
        printf("小端存储!\n");
    }
    else
    {
        printf("大端存储!\n");
    }
#endif
    
    return 0;
}
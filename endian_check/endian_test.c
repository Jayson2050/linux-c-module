/*
 * @Descripttion: Test one machine's endian is BIG or not.
 * @version: 
 * @@Company: XEL
 * @Author: shenhaojie
 * @Date: 2022-03-18 10:01:46
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-03-18 10:04:54
 */
#include <stdio.h>

int main()
{
    int a = 0x11223344;
    char * p = (char *)&a;

    if (*p == 0x44)
    {
        printf("Little-Endian!\r\n");
    }
    else if(*p == 0x11)
    {
        printf("Big-Endian\r\n");
    }

    return 0;
}
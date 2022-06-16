/*
 * @Descripttion:
1、函数介绍
void *memset(void *s, int ch, size_t n);
函数解释：将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 。
memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法 [1]  。
memset()函数原型是extern void *memset(void *buffer, int c, int count) buffer：为指针或是数组,c：是赋给buffer的值,count：是buffer的长度.

2、常见错误
第一：memset函数按字节对内存块进行初始化，所以不能用它将int数组初始化为0和-1之外的其他值（除非该值高字节和低字节相同）。
第二：memset(void *s, int ch,size_t n);中ch实际范围应该在0~~255，因为该函数只能取ch的后八位赋值给你所输入的范围的每个字节，比如int a[5]赋值memset（a,-1,sizeof(int )*5）与memset（a,511,sizeof(int )*5） 所赋值的结果是一样的都为-1；因为-1的二进制码为（11111111 11111111 11111111 11111111）而511的二进制码为（00000000 00000000 00000001 11111111）后八位都为（11111111)，所以数组中每个字节，如a[0]含四个字节都被赋值为（11111111），其结果为a[0]（11111111 11111111 11111111 11111111），即a[0]=-1，因此无论ch多大只有后八位二进制有效，而后八位二进制的范围在（0~255）中改。而对字符数组操作时则取后八位赋值给字符数组，其八位值作为ASCII码。
第三： 搞反了 ch 和 n 的位置.
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-06-16 22:20:18
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-06-16 21:44:23
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>


#define SIZE    10

int main(void)
{
    int i;
    int a[SIZE];
    unsigned int b[SIZE];

    memset(a, 0, sizeof(a));
    printf("memset 0 test:\r\n");
    for(i = 0; i < SIZE; i++)
    {
        printf("a[%d] = %d \r\n", i, a[i]);
    }

    memset(a, -1, sizeof(a));
    printf("memset -1 test:\r\n");
    for(i = 0; i < SIZE; i++)
    {
        printf("a[%d] = %d \r\n", i, a[i]);
    }

    memset(a, 1, sizeof(a));
    printf("memset 1 test:\r\n");
    for(i = 0; i < SIZE; i++)
    {
        printf("a[%d] = %d \r\n", i, a[i]);
    }
    
    memset(a, 0x55, sizeof(a));
    printf("memset 0x55 test:\r\n");
    for(i = 0; i < SIZE; i++)
    {
        printf("a[%d] = %d \r\n", i, a[i]);
    }


    memset(b, -1, sizeof(b));
    printf("int16 b[], memset -1 test:\r\n");
    for(i = 0; i < SIZE; i++)
    {
        printf("b[%d] = %d \r\n", i, b[i]);
    }

    return 0;
}
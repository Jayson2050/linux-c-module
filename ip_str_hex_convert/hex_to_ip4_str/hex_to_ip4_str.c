/*
 * @Descripttion: 
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-05-31 18:28:28
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-06-09 14:51:14
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <assert.h>
#include <stddef.h>


typedef unsigned char      uint8;   /* 8-bit quantity  */
typedef unsigned short     uint16;  /* 16-bit quantity */
typedef unsigned int       uint32;  /* 32-bit quantity */

typedef signed char        int8;    /* 8-bit quantity  */
typedef signed short       int16;   /* 16-bit quantity */
typedef signed int         int32;   /* 32-bit quantity */


unsigned int change1(unsigned int ip, char *mm)
{
	sprintf(mm, "%d.%d.%d.%d", (unsigned char)(ip>>24), (unsigned char)(ip>>16), (unsigned char)(ip>>8),(unsigned char)ip );
}
unsigned int change2(unsigned int ip, char *mm)
{
	sprintf(mm, "%d.%d.%d.%d", (ip & 0xff000000)>>24, ip & 0xff0000)>>16, (ip & 0xff00)>>8,(ip & 0xff);
}
unsigned int change3(unsigned int ip, char *mm)
{
	sprintf(mm, "%d.%d.%d.%d", (ip>>24) & 0xff, (ip>>16) & 0xff, (ip>>8) & 0xff, ip &0xff);
}
int main()
{
	char message[32] = {0};
	change1(0xc0a8019b, message);
	printf("10进制的ip地址是:%s\r\n", message);
	return 0;
}


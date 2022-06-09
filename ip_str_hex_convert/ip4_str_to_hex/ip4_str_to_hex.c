/*
 * @Descripttion: 
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-05-31 18:28:28
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-06-09 14:37:12
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

unsigned int Covert(char *message)
{
	char *p[4] = {0};
	char *last;
	char * buf = message;
	
	p[0] = strtok_r(buf, ".", &last);
	printf("p[0] = %s\r\n", p[0]);
	p[1] = strtok_r(NULL, ".", &last);
	printf("p[1] = %s\r\n", p[1]);
	p[2] = strtok_r(NULL, ".", &last);
	printf("p[2] = %s\r\n", p[2]);
	p[3] = strtok_r(NULL, ".", &last);
	printf("p[3] = %s\r\n", p[3]);
	return (atoi(p[0]) << 24) | (atoi(p[1]) << 16) | (atoi(p[2])<<8) | (atoi(p[3]));
}


int main()
{
	char message[32] = {0};
	strcpy(message, "192.168.1.155 ");
	printf("10进制的ip地址是: %s\r\n ", message);
	printf("16进制的ip地址是: %#x\r\n ", Covert(message));
	return 0;
}

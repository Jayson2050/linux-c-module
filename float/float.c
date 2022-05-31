/*
 * @Descripttion: 
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-05-31 18:28:28
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-05-31 17:54:32
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <unistd.h>
#include <assert.h>
#include <stddef.h>


typedef unsigned char      uint8;   /* 8-bit quantity  */
typedef unsigned short     uint16;  /* 16-bit quantity */
typedef unsigned int       uint32;  /* 32-bit quantity */

typedef signed char        int8;    /* 8-bit quantity  */
typedef signed short       int16;   /* 16-bit quantity */
typedef signed int         int32;   /* 32-bit quantity */

void main( void )
{
    float a1 = 0;
	float a2 = 0.0;
	float rlt = 0.0;
	a1 = 10.0;
	a2 = 5.0;
    rlt = a1/a2;
	printf( "===1 10.0/5.0 = %f\r\n", rlt);

	rlt = a1/2;
	printf( "===2 10.0/2 = %f\r\n", rlt);

	printf( "\n");
	
}
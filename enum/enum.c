/*
 * @Descripttion: 
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-05-31 18:28:28
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-07-11 18:11:51
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

typedef enum 
{
	A = 0,
	B = 1,
	C = 2,
}enum_1_t;

typedef enum 
{
	EE = -1,
	D = 0,
	E = 1,
	F = 2,
}enum_2_t;

enum_1_t fun1(void)
{
	return -1;
}

enum_2_t fun2(void)
{
	return -1;
}

void main( void )
{
    int32 a1, a2;
	uint32 a3, a4;

	a1 = fun1();
	printf( "a1 = %d\r\n", a1);
	a2 = fun2();
	printf( "a2 = %d\r\n", a2);
	
	a3 = fun1();
	printf( "a3 = %d\r\n", a3);
	a4 = fun2();
	printf( "a4 = %d\r\n", a4);

	printf( "fun1 = %d\r\n", fun1());
	printf( "fun2 = %d\r\n", fun2());

	if(fun1() < 0)
	{
		printf( "fun1 < 0, fun1 = %d\r\n", fun1());
	}
	else
	{
		printf( "fun1 > 0, fun1 = %d\r\n", fun1());
	}


	if(fun2() < 0)
	{
		printf( "fun2 < 0, fun1 = %d\r\n", fun2());
	}
	else
	{
		printf( "fun2 > 0, fun1 = %d\r\n", fun2());
	}
	
	if(fun1() == -1)
	{
		printf( "fun1 == -1, fun1 = %d\r\n", fun1());
	}
	else
	{
		printf( "fun1 != -1, fun1 = %d\r\n", fun1());
	}
}
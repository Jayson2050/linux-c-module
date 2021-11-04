#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <unistd.h>
#include <assert.h>
#include <stddef.h>

#include <pthread.h>

typedef unsigned char      uint8;   /* 8-bit quantity  */
typedef unsigned short     uint16;  /* 16-bit quantity */
typedef unsigned int       uint32;  /* 32-bit quantity */

typedef signed char        int8;    /* 8-bit quantity  */
typedef signed short       int16;   /* 16-bit quantity */
typedef signed int         int32;   /* 32-bit quantity */

/* 结果值将含a不含b，[a, b) */
#define random_1(a,b) ((rand()%(b-a))+a)
/* 结果值将含a含b,[a, b] */
#define random_2(a,b) ((rand()%(b-a+1))+a)
 
void main( void )
{
    int i=RAND_MAX;
    
	printf("RAND_MAX=0x%x   time(0)=%d \n",RAND_MAX,(int)time(0));
    srand((int)time(NULL)); 
    /* 输出 10 个随机数. */
	printf( "[0,100): ");
    for( i = 0;   i < 10;i++ )
    {   
 		printf( "random = %6d  \n", random_1(0,3) );
	}

    printf( "[0,100]: ");
	for( i = 0;   i < 10;i++ )
    {   
 		printf( "random = %6d  \n", random_2(0,3) );
	}
	printf( "\n");
	
}
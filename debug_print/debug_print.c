#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>


#define DBG_PRINT(flag, fmt, args...)   do{ \
                                            if(flag)\
                                            {\
                                                printf(fmt, ##args);\
                                            }\
                                        }while(0)

void main() 
{
	int flag = 1;
    unsigned int a = 1, b=2; 
    DBG_PRINT(flag, "test 0x%02x 0x%02x\r\n", a,b);
	return;
}


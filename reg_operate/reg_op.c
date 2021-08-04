#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

typedef unsigned char      uint8;   /* 8-bit quantity  */
typedef unsigned short     uint16;  /* 16-bit quantity */
typedef unsigned int       uint32;  /* 32-bit quantity */

typedef signed char        int8;    /* 8-bit quantity  */
typedef signed short       int16;   /* 16-bit quantity */
typedef signed int         int32;   /* 32-bit quantity */

/* Get 32bit reg bits member data */ 
#define SOC_GET_BITS_32(val, b_low, b_high)  (((val) >> (b_low)) & ((1 << ((b_high) - (b_low) + 1)) - 1))

#define SHIFT_MASK(n)   ((1ull << (n)) - 1)
#define SOC_SET_BITS_32(val, b_low, b_high)  (((val) << (b_low)) & SHIFT_MASK((b_high) + 1))


#define DBG_PRINT(flag, fmt, args...)   do{ \
                                            if(flag)\
                                            {\
                                                printf(fmt, ##args);\
                                            }\
                                        }while(0)

void main() 
{
	int flag = 1;
    uint32 tmp= 0xffff, data;

    
    DBG_PRINT(flag, "data = 0x%08x \r\n", data);
	return;
}


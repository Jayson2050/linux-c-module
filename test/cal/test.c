#include <stdio.h>
#include <string.h>

typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned char uint8;

static uint get_data(uint index)
{
    uint a = 0x06000000;
    uint ret, tmp;
    tmp = index*16 +index;
    ret = a + tmp;
    return ret;
}

int main(void)
{
	int ret,i;
	for(i=0;i<6;i++)
    {
        ret = get_data(i);
        printf("ret=%#08x\r\n",ret);
    } 
	return 0;
}

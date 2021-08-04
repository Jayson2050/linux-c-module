#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define WORD_INVERT(x)  (((x) << 24) | \
                        (((x) &  0x0000ff00) << 8) | \
                        (((x) &  0x00ff0000) >> 8) | \
                        ((x)  >> 24))

#define T(x) (((x&0xff)<<24)|((x&0xff00)<<8)|((x&0xff0000)>>8)|((x&0xff000000)>>24))

int main(int argc,char *argv[])
{
	
	unsigned int a = 0x11223344;
    // a = T(a);
    a = WORD_INVERT(a);
	printf("a=0x%08x\r\n",a);

	return 0;
}

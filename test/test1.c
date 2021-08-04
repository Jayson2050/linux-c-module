#include <stdio.h>
#include <string.h>

typedef struct test_s
{
	int a;
	int b;
	int c;
	unsigned char d;
	int e;
}test_t;

char tmp[30] = {0};
char *my_cpy(char* src, int len)
{

	memcpy(tmp,src,len);
 
	return tmp;
}


int main(void)
{
	int ret =0;
	char *tmp[20] ={0};
	char *p;
	test_t tt,aa;
 
	tt.a = 0xa;
	tt.b = 0xbb;
	tt.c = 0xccc;
	tt.d = 0xdd;
	tt.e = 0xeeee;
 
	p = my_cpy((char *)&tt, sizeof(tt));
	aa = *(test_t *)p;
 
	printf("aa.a=%x\r\n",aa.a);
	printf("aa.b=%x\r\n",aa.b);
	printf("aa.c=%x\r\n",aa.c);
	printf("aa.d=%x\r\n",aa.d);
	printf("aa.e=%x\r\n",aa.e);
 
	return 0;
}


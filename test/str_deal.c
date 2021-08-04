#include <stdio.h>
#include <string.h>
#include <malloc.h>
int main(int argc,char *argv[])
{
	int i;
//	char tmp[100] = {0};	
//	char a[8][80] = {"aaaaaaaaaaaaaa","bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb","cccc","dddd"};
//	char a[100];
//	unsigned char b[100]="ggggg";
//	char *p=(char *)malloc(5*2);
//	printf("malloc len:%d\r\n",sizeof(*p));
//	sprintf(tmp," %10s",b);
//	printf("tmp=%010s",tmp);
//	strcat(a,tmp);
//	printf("a=%010s\r\n",a);
//	printf("a[0]=%10x\r\n",a[0]);
//	bf = a[0];
//	printf("bf[0]=%s\r\n",bf);
//	bf = a[1];
//	printf("bf[80]=%s\r\n",bf);
	typedef struct
	{
		int a;
		int b;
	}test_t;
	test_t tmp;
	test_t *p1 =(test_t *)malloc(sizeof(test_t)*10);
	test_t *p[20]={0};
	//printf("sizeof(tmp)=%d\r\n",sizeof(tmp));
	//printf("sizeof(p)=%d\r\n",sizeof(p));
	//printf("sizeof(*p)=%d\r\n",sizeof(*p));
	//printf("sizeof(p1)=%d\r\n",sizeof(p1));

	
	int a = 0x3340;
	printf("aaaa=0x%03x\r\n",a);
	printf("aaaa=0x%04x\r\n",a);

	return 0;
}

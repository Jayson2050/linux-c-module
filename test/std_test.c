#include <string.h>
#include <stdio.h>

int main(void)
{
	unsigned char input[30]="  dbg-read  0x11223344    10";
	char *p,str;
	unsigned int data;
	/*strtok places a NULL terminator infront of the token,if found*/
	p=strtok(input," ");
	if(p)
	   printf("%s\n",p);
	/*Asecond call to strtok using a NULL as the first parameter returns a pointer to the character following the token*/
    p=strtok(NULL," ");
    if(p)
	{ 
		printf("addr(str)=%s\n",p);
		sscanf(p,"%x",&data);
		printf("addr(hex)=%04x\n",data);
		printf("addr(int)=%d\n",data);
		sscanf(data,"%s",str);
		printf("int to string:%s",str);
	}
	p=strtok(NULL," ");
    if(p)
        printf("%s\n",p);
    return 0;
}

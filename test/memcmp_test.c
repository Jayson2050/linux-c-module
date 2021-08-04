#include <stdio.h>
#include <string.h>
#include <malloc.h>


typedef struct
    {
        int a;
        char b[10];
    }test_t;

int main(int argc,char *argv[])
{
	int ret;
    test_t t1 = {0};
    test_t t2 = {0};
    t1.a = 1;
    t1.b[0] = 1;

    t2.a = 2;
    t2.b[0] = 1;

    ret = memcmp(&t1, &t2, sizeof(test_t));
    printf("ret = %d\n", ret);

	return 0;
}

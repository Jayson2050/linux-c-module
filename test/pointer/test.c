#include <stdio.h>
#include <malloc.h>

typedef struct test1
{
    int a;
    char *b;

}test1_t;

typedef struct test2
{
    int a;
    char *b;
    test1_t *p;

}test2_t;

void f1(test2_t *p)
{
    test2_t *ptmp = (test2_t *)malloc(sizeof(test2_t));
    if(ptmp == NULL)
    {
        printf("error malloc\r\n");
    }
    else
    {
        printf("successfully malloc\r\n");
    }
    ptmp->b = "hello";
    p->b = ptmp->b; //ok
}

void f2(test2_t *p)
{
    static test2_t ptmp;

    ptmp.b = "hello";
    p = &ptmp;   //ok, 因为函数内无论结构变量还是普通变量，在其之前加上static就构成了static变量，其在内存中为全局静态区，编译的时候就分配内存给它了，
                //如果不加static，那就是普通局部变量，执行完就被销毁
}

int main(void)
{
    test2_t tt;
    f1(&tt);
    printf("f1, tt.b=%s\r\n",tt.b);
    f2(&tt);
    printf("f2, tt.b=%s\r\n",tt.b);
    return 0;
}
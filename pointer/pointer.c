/*
 * @Descripttion: 测试目的：函数形参传入空结构体指针和非空结构体指针的相关行为
    1. 传入空结构体一级指针ptr1时，①函数内不能操作结构体成员；②该函数fun1又调用其他函数fun2且fun2返回一个结构体指针ptr2(malloc分配了内存)，
    此时ptr2只能在fun1中使用，如果将ptr1指向ptr2，最后fun1是无法将ptr2通过一级指针返回的；
    2. 传入已分配内存空间的一级指针ptr1(此处主要测试结构体指针)，①该函数fun1可以操作结构体成员；②同第1点中的②描述
    3. 传入空结构体二级指针ptr1时，①函数内不能操作结构体成员；②该函数fun1又调用其他函数fun2且fun2返回一个结构体指针ptr2(malloc分配了内存)，
    此时ptr2只能在fun1中使用，如果将*ptr1指向ptr2，最后fun1是可以将ptr2通过二级指针返回的；
    4. 传入已分配内存空间的二级指针ptr1(此处主要测试结构体指针)，①该函数fun1可以操作结构体成员；②同第3点中的②描述
    其中第4点中②操作最好不好不要使用，因为已分配内存的指针又重新指向了其他内存，这这样之前的内存没有释放会导致内存泄漏。
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-04-14 20:10:19
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-04-15 09:26:02
 */
#include <stdio.h>
#include <malloc.h>

typedef struct struct_1_s
{
    int data;
    const char *str;

}struct_1_t;


typedef struct struct_2_s
{
    int data;
    const char *str;
    struct_1_t *ptr;

}struct_2_t, *struct_2_tp;


void fun1(struct_2_t *ptr)
{
    struct_2_t *ptmp = (struct_2_t *)malloc(sizeof(struct_2_t));

    if(ptmp == NULL)
    {
        printf("error malloc\r\n");
    }
    else
    {
        printf("successfully malloc\r\n");
    }
    ptmp->str = "This is fun1";
    ptr->str = ptmp->str; //ok
}

void fun2(struct_2_t *ptr)
{
    static struct_2_t ptmp;

    ptmp.str = "This is fun2";
    ptr = &ptmp;   //ok, 因为函数内无论结构变量还是普通变量，在其之前加上static就构成了static变量，其在内存中为全局静态区，编译的时候就分配内存给它了，
                //如果不加static，那就是普通局部变量，执行完就被销毁
}

struct_2_tp test_create(const char *desc)
{
    struct_2_t *ptr_tmp = (struct_2_t *)malloc(sizeof(struct_2_t));
    ptr_tmp->str = desc;
    return ptr_tmp;
}

/* 传入结构体指针，然后从其他函数获取一个指针 */
void fun3(struct_2_t *ptr)
{
	struct_2_t *ptr_local = NULL;
    ptr_local = test_create("This is fun3");
	ptr = ptr_local; // 局部变量在函数运行结束就被销毁，因此ptr无法获ptr_local指向的内存地址
	// ptr = test_create("This is fun3"); // 该行等价于上面的处理，因此传入fun3的指针在调用函数后依然是无法从fun3中获取到内存指针的
    printf("fun3 内部从其他函数返回获取到的ptr=%p\r\n", ptr);
}

/* 传入结构体指针，然后对结构体成员进行赋值 */
void fun4(struct_2_t **ptr)
{
    *ptr = test_create("This is fun4");
    printf("fun4 内部从其他函数返回获取到的*ptr=%p\r\n", *ptr);
    printf("fun4 内部从其他函数返回获取到的ptr=%p\r\n", ptr);
}

int main(void)
{
    int test_id = 1;
    struct_2_t test1;
    struct_2_tp test2 = NULL;
    static struct_2_tp test3 = NULL;
    struct_2_t *test4 = (struct_2_t *)malloc(sizeof(struct_2_t));


    printf("========== test %d ========== \r\n", test_id++);
    /* test1 */
    fun1(&test1);
    printf("test1.str=%s\r\n",test1.str);

    printf("========== test %d ========== \r\n", test_id++);
    /* test2 */
    fun2(&test1);
    printf("test1.str=%s\r\n",test1.str);
    
    printf("========== test %d ========== \r\n", test_id++);
    /* 测试函数传入NULL结构体指针后能否返回数据 */
    fun3(test2);
    if(test2 == NULL)
    {
        printf("pointer is NULL\r\n");
    }
    else
    {
        printf("test2.str=%s\r\n",test2->str);
        printf("pointer address=0x%p\r\n", test2);
    }
    
    printf("========== test %d ========== \r\n", test_id++);
    /* 测试函数传入NULL结构体指针后能否返回数据 */
    fun3(test3);
    if(test3 == NULL)
    {
        printf("pointer is NULL\r\n");
    }
    else
    {
        printf("test3.str=%s\r\n",test3->str);
        printf("pointer address=0x%p\r\n", test3);
    }

    printf("========== test %d ========== \r\n", test_id++);
    /* 测试函数传入已分配内存的结构体指针后能否返回数据，预期函数可以修改该内存数据 */
    printf("1 pointer address=0x%p\r\n", test4);
    fun1(test4);
    if(test4 == NULL)
    {
        printf("test4 pointer is NULL\r\n");
    }
    else
    {
        printf("test4.str=%s\r\n",test4->str);
        printf("2 pointer address=0x%p\r\n", test4);
    }

    printf("========== test %d ========== \r\n", test_id++);
    /* 测试函数传入已分配内存的结构体指针后能否返回数据， */
    printf("1 pointer address=0x%p\r\n", test4);
    fun3(test4);
    if(test4 == NULL)
    {
        printf("pointer is NULL\r\n");
    }
    else
    {
        printf("test4.str=%s\r\n",test4->str);
        printf("2 pointer address=0x%p\r\n", test4);
    }

    printf("========== test %d ========== \r\n", test_id++);
    /* 测试函数传入已分配内存的结构体指针后能否返回数据， */
    test4 = NULL;   //应该使用free后再赋值NULL
    fun3(test4);
    if(test4 == NULL)
    {
        printf("pointer is NULL\r\n");
    }
    else
    {
        printf("test4.str=%s\r\n",test4->str);
        printf("2 pointer address=0x%p\r\n", test4);
    }

    printf("========== test %d ========== \r\n", test_id++);
    /* 测试函数传入NULL结构体指针后能否返回数据，预期可以返回指针 */
    printf("1 pointer address=0x%p\r\n", test3);
    fun4(&test3);
    if(test3 == NULL)
    {
        printf("pointer is NULL\r\n");
    }
    else
    {
        printf("test3.str=%s\r\n",test3->str);
        printf("2 pointer address=0x%p\r\n", test3);
    }

    printf("========== test %d ========== \r\n", test_id++);
    /* 测试函数传入已分配内存的结构体指针后能否返回数据, 该方式会导致内存泄漏，
    因为指向之前malloc分配内存的指针没有free的情况下又指向了其他内存 */
    printf("1 pointer address=0x%p\r\n", test4);
    fun4(&test4);
    if(test4 == NULL)
    {
        printf("pointer is NULL\r\n");
    }
    else
    {
        printf("test4.str=%s\r\n",test4->str);
        printf("2 pointer address=0x%p\r\n", test4);
    }

    return 0;
}
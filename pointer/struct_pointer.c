/*
 * @Descripttion: 
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-07-08 14:28:33
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-07-08 13:49:12
 */
/*
 * @Descripttion: 测试目的：函数形参传入空结构体指针和非空结构体指针的相关行为
    1. 传入空结构体一级指针ptr1时，①函数内不能操作结构体成员；②该函数fun1又调用其他函数fun2且fun2返回一个结构体指针ptr2(malloc分配了内存)，
    此时ptr2只能在fun1中使用，如果将ptr1指向ptr2，最后fun1是无法将ptr2通过一级指针返回的；
    2. 传入已分配内存空间的一级指针ptr1(此处主要测试结构体指针)，①该函数fun1可以操作结构体成员；②同第1点中的②描述
    3. 传入空结构体二级指针ptr1时，①函数内不能操作结构体成员；②该函数fun1又调用其他函数fun2且fun2返回一个结构体指针ptr2(malloc分配了内存)，
    此时ptr2只能在fun1中使用，如果将*ptr1指向ptr2，最后fun1是可以将ptr2通过二级指针返回的；
    4. 传入已分配内存空间的二级指针ptr1(此处主要测试结构体指针)，①该函数fun1可以操作结构体成员；②同第3点中的②描述
    其中第4点中②操作最好不好不要使用，因为已分配内存的指针又重新指向了其他内存，这这样之前的内存没有释放会导致内存泄漏。
    5. 容易出错的一个点是通过&取结构体变量地址传入函数形参时，实际上传入的仅仅是地址的一份镜像，函数内部修改该地址后数据是不会返回给该地址结构体的，
    解决方案1：定义结构体变量时，同时定义其结构体指针，然后将该指针指向该结构体变量的地址，传入该结构体指针就能正确返回数据了。
    解决方案2：定义结构体指针，然后通过malloc等方式为该指针申请内存，最后传入该结构体指针就能正确返回数据了，也就是说传入函数形参的结构体地址只能是指针，
            而不是&取结构体变量地址。
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-04-14 20:10:19
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-05-27 09:44:09
 */
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define INFO    "v1.0.0.0"

typedef struct struct_2_s
{
    int data;
    const char *str;

}struct_2_t, *struct_2_tp;



/* 传入结构体指针，然后对结构体成员进行赋值，包含memcpy方式 */
void fun5(struct_2_t *ptr)
{
    int data = 0x11223388;
    memcpy(&ptr->data, &data, sizeof(data));
    ptr->str = "get a string";
}

/* 获取字符串信息 */
void fun6(char **str)
{
    *str = INFO;
}

const char * get_string_info(void)
{
    return INFO;
}

#define TEST_NUM    100000

int main(void)
{
    int test_id = 1;
    int i = 0;
    char *str_info = NULL;
    struct_2_t *test5 = (struct_2_t *)malloc(sizeof(struct_2_t));

    for(i = 0; i < 1 ; i++)
    {
        printf("========== test %d ========== \r\n", test_id++);
        /* 测试函数传入已分配内存的结构体指针后能否返回数据, 该方式会导致内存泄漏，
        因为指向之前malloc分配内存的指针没有free的情况下又指向了其他内存 */
        fun5(test5);
        if(test5 == NULL)
        {
            printf("pointer is NULL\r\n");
        }
        else
        {
            // printf("test5.data=%#08x\r\n",test5->data);
            // printf("test5.str=%s\r\n",test5->str);
            // printf("2 pointer address=0x%p\r\n", test5);
            if(test5->data != 0x2244671*8)
            {
                printf("test error!\r\n");
            }
            else
            {
                printf("test ok!\r\n");
            }
        }
        
    }
    // fun6(&str_info);
    str_info = get_string_info();
    printf("string is %s \r\n", str_info);

    free(test5);
    test5 = NULL;
    return 0;
}
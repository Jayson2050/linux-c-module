/*
 * @Descripttion: 
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-03-21 11:35:37
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-03-21 12:01:15
 */
#include <stdio.h>
#include <malloc.h>


typedef unsigned int uint32_t;

void fun(uint32_t addr, char * data, uint32_t * tmp)
{
    char * array[12] = {0};
    uint32_t *p = &addr;
    printf("addr=%d, data=%d, tmp=%d\r\n", *p, *(uint32_t *)(data + 0), tmp[0]);
    *p += 1;
    *tmp +=1; 
}


int main(void)
{
    char array_data[12] = {1,2,3,4,5,6,7,8};
    uint32_t local_addr = 10, tmp = 20;
    
    printf("local_addr = %d\r\n", local_addr);
    
    fun(local_addr, array_data, &tmp);

    printf("local_addr=%d,array_word1=%d, tmp=%d\r\n", local_addr, *(uint32_t *)(array_data + 4), tmp);
    return 0;
}
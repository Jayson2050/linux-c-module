/*
 * @Descripttion:模拟libc接口的源码实现
 * @version: 
 * @@Company: DCIT-SH
 * @Author: shenhaojie
 * @Date: 2022-06-16 22:20:18
 * @LastEditors: shenhaojie
 * @LastEditTime: 2022-06-16 21:44:23
 */


#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>

#define _CRT_SECURE_NO_WARNINGS 1

enum Status
{
	VAILD,
	INVAILD//非法的
}status = INVAILD;//初始化为非法

int my_atoi(const char* str)
{
	assert(str);
	if (*str == '\0')//空字符串问题
	{
		return 0;
	}
	//+-号问题
	//用isspace函数向后过滤空白字符
	//isspace返回值不为0，代表是空格
	while (isspace(*str))
	{
		str++;
	}//走到这里str一定不是空格了
	
	int flg = 1;
	if (*str == '+')
	{
		flg = 1;
		str++;
	}//注意这里要用else if，不能用else
	else if(*str == '-')
	{
		flg = -1;
		str++;
	}
	long long n = 0;
	while (*str != '\0')
	{
		if (isdigit(*str))
		{//是数字字符==>返回值不为0
			n = n * 10 + (*str - '0') * flg;
			if (n > INT_MAX)
			{
				n = INT_MAX;
			}
			if (n < INT_MIN)
			{
				n = INT_MIN;
			}
		}
		else//不是数字字符
		{
			return (int)n;
		}
		str++;
	}
	if (*str == '\0')
	{
		status = VAILD;//走到‘\0’就是合法转换
	}
	return (int)n;
}
int main()
{
	int ret = my_atoi("-123");
	if (status == VAILD)
	{
		printf("合法转换: n = %d\n",ret);
	}
	else
	{
		printf("非法转换: n = %d\n",ret);
	}
	return 0;
}
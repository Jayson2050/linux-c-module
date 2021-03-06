#pragma once

#include "顺序栈-入栈出栈.h"
#include <string.h>

//定义入栈函数
int push(char* a,int top,char elem)
{

	a[++top]=elem;
	return top;
}

//定义出栈函数
int pop(char* a,int top)
{
	if (top == -1)
	{
		printf_s("栈空!\n");
		return -1;
	}

	printf_s("出栈元素：%c\n",a[top]);
	top --;
	return top;
}

//入出栈配置
int Stack_Test()
{
	char a[100];
	int top=-1;


	//if (top<(sizeof(a)/sizeof(char))) //这种方法不行
	if (top>(int)strlen(a))				//使用strlen()函数可行！
	{
		printf_s("栈溢出!\n");
		return -1;
	}
	//printf_s("%d\n",sizeof(a)/sizeof(char));
	top=push(a, top, 'a'); //top = 0
	top=push(a, top, 'b'); //top = 1
	top=push(a, top, 'c'); //top = 2
	top=push(a, top, 'd'); //top = 3
	top=pop (a, top); //a[3],top = 2
	top=pop (a, top); //a[2],top = 1
	top=pop (a, top); //a[1],top = 0
	top=pop (a, top); //a[0],top = -1
	top=pop (a, top); //"栈溢出"，top = -1


	return 0;
}
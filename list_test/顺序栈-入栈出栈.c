#pragma once

#include "˳��ջ-��ջ��ջ.h"
#include <string.h>

//������ջ����
int push(char* a,int top,char elem)
{

	a[++top]=elem;
	return top;
}

//�����ջ����
int pop(char* a,int top)
{
	if (top == -1)
	{
		printf_s("ջ��!\n");
		return -1;
	}

	printf_s("��ջԪ�أ�%c\n",a[top]);
	top --;
	return top;
}

//���ջ����
int Stack_Test()
{
	char a[100];
	int top=-1;


	//if (top<(sizeof(a)/sizeof(char))) //���ַ�������
	if (top>(int)strlen(a))				//ʹ��strlen()�������У�
	{
		printf_s("ջ���!\n");
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
	top=pop (a, top); //"ջ���"��top = -1


	return 0;
}
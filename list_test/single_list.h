#ifndef __SINGLE_LIST__
#define __SINGLE_LIST__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#pragma once

//链表的节点
typedef struct list
{
    int data;
    struct list* next;
}list_t,*plist_t;

/* 封装了链表的结构 */
typedef struct node 
{
    plist_t phead;//指向链表第一个节点
}node_t;

//链表的初始化
void list_init(node_t *s);

struct list* alloc_node(int data);

//在链表s最后一个节点后插入一个值为data的节点
void list_tail_insert_node(node_t* s, int data);

//删除链表s最后一个节点
void list_tail_del_node(node_t* s);

//在链表s第一个节点前插入值为data的节点
void list_head_insert_node(node_t* s, int data);

//删除链表s的第一个节点
void list_head_del_node(node_t* s);

// 在链表中查找值为data的节点，找到返回该节点的地址，否则返回NULL 
plist_t list_find_node(node_t* s, int data);

//在链表的pos位置后插入值为data的节点
void list_insert_node_via_position(plist_t pos, int data);

//删除链表s中pos位置的节点
void list_del_node(node_t* s, plist_t pos);

//移除链表中第一个值为data的元素
void list_remove_data(node_t*s, int data);

// 获取链表中有效节点的个数 
int get_list_node_cnt(node_t* s);

// 检测链表是否为空 
int list_empty_check(node_t* s);

// 将链表中有效节点清空 
void list_purge(node_t* s);

// 销毁链表 
void list_destroy(node_t* s);
//打印链表
void list_print(node_t* s);

#endif /* __SINGLE_LIST__ */
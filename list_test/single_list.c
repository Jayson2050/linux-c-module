#include "single_list.h"

/* 申请一个结点，并给数据域赋值，返回该结点地址 */
struct list* alloc_node(int data)
{ 
	struct list* p;
	p = malloc(sizeof(struct list));
	p->data = data; 
	p->next = NULL; 
	return p;
}

/* 1.初始化部分，我们只需要将链表的头结点置为NULL即可 */
void list_init(node_t *s) 
{
	assert(s);
	s->phead = NULL;
}

/* 2.尾插，首先我们要创建一个新节点，然后判断链表当前是否有节点，
若没有，则直接让第一个节点指向新节点，若有，找到最后一个节点，让他指向新节点。 */

void list_tail_insert_node(node_t* s, int data) 
{
	//找链表最后一个节点
	assert(s);
	plist_t pNewNode = alloc_node(data);
	if (s->phead == NULL) //链表没有节点的情况（头结点为空）
	{
		s->phead = pNewNode;
	}
	else //链表有节点的情况，即头结点不为空，需要遍历链表，寻找链表最后一个结点
	{
		plist_t pCur = s->phead;
		while (pCur->next) //遍历链表，寻找最后一个结点，通过判断next是否为空来检查
		{
			pCur = pCur->next; //将当前结点指针指向下一个结点地址处
		}
		//让最后一个节点指向新节点
		pCur->next = pNewNode;
	}
}

/* 3.尾删，首先判断链表中有没有节点，若没有，直接返回，若有一个节点，
直接让第一个节点指向NULL，若有多个节点，则需要记录下倒数第二个节点，让它指向NULL。 */
void list_tail_del_node(node_t* s) 
{
	assert(s);
	if (s->phead == NULL) 
	{//链表中没有节点
		return;
	}
	else if (s->phead->next == NULL) 
	{//只有一个节点
		free(s->phead);
		s->phead = NULL;
	}
	else //多个节点
	{
		plist_t pCur = s->phead;
		plist_t pPre = NULL;
		while (pCur->next) {
		pPre = pCur;
		pCur = pCur->next;
	}
	free(pCur);
	pPre->next = NULL;
	}
}

/* 4.头插 */
void list_head_insert_node(node_t* s, int data) 
{
	assert(s);
	plist_t pNewNode = alloc_node(data);
	if (s->phead == NULL) 
	{//链表为空
		s->phead = pNewNode;
	}
	else 
	{
		pNewNode->next = s->phead;
		s->phead = pNewNode;
	}
}

/* 5.头删 */
void list_head_del_node(node_t* s) 
{
	assert(s);
	if (s->phead == NULL) 
	{//链表为空
		return;
	}
	else if (s->phead->next == NULL) 
	{//只有一个节点
		free(s->phead);
		s->phead = NULL;
	}
	else 
	{
		plist_t pCur = s->phead;
		s->phead = pCur->next;
		free(pCur);
	}
}

/* 6.在给定pos位置插入值为data的节点，
分两步完成：首先找到pos位置的节点，然后再插入，所以要实现这一个功能需要两个函数来共同完成。 */

//查找：
plist_t list_find_node(node_t* s, int data) 
{
	assert(s);
	plist_t pCur = s->phead;
	while (pCur) 
	{
		if (pCur->data == data) 
		{
			return pCur;
		}
		pCur = pCur->next;
	}
	return NULL;
}

//插入(注意：应该先连接好新节点，再断开原来的指针指向)：
void list_insert_node_via_position(plist_t pos, int data) 
{
	plist_t pNewNode = NULL;
	if (pos == NULL) 
	{
		return;
	}
	pNewNode = alloc_node(data);

	pNewNode->next = pos->next;
	pos->next = pNewNode;
}

/* 7.删除给定pos位置的节点。 */
void list_del_node(node_t* s, plist_t pos) 
{
	assert(s);
	if (pos == NULL || s->phead == NULL) 
	{
		return;
	}
	if (pos== s->phead) 
	{
		s->phead = pos->next;
	}
	else 
	{
		plist_t pPrePos = s->phead;
		while (pPrePos&&pPrePos->next != pos) 
		{
			pPrePos = pPrePos->next;
		}
		pPrePos->next = pos->next;
	}
	free(pos);
}

/* 8.删除第一个值为data的节点。要分三种情况：链表为空直接返回、要删除的节点为第一个节点、其它位置的节点。 */
void list_remove_data(node_t *s, int data) 
{
	assert(s);
	if (s->phead == NULL) 
	{
		return;
	}
	plist_t pPre = NULL;
	plist_t pCur = s->phead;
	while (pCur) 
	{
		if (pCur->data == data) 
		{
			if (pCur == s->phead) 
			{  //要删除的是第一个位置的节点
				s->phead = pCur->next;
			}
			else 
			{
				pPre->next = pCur->next; //其它位置的情况，让前一个节点指向其后一个节点
			}
			free(pCur);
			return;
		}
		else 
		{
			pPre = pCur;
			pCur = pCur->next;
		}
	}
}
/* 获取链表有效节点的个数 */
int get_list_node_cnt(node_t* s) 
{ 
	assert(s);
	int count = 0;
	plist_t pCur = s->phead;
	while (pCur) 
	{
		count++;
		pCur = pCur->next;
	}
	return count;
}

/* 检查链表是否为空 */
int list_empty_check(node_t* s) 
{
	assert(s);
	if (s->phead == NULL) 
	{
		return -1;
	}
	return 0;
}

/* 清空链表 */
void list_purge(node_t* s) 
{             
	assert(s);
	if (s->phead == NULL) 
	{
		return;
	}
	plist_t pCur = s->phead;
	while (pCur->next) 
	{ //循环清空链表中的节点
		plist_t Tmp = pCur->next;
		free(pCur);
		pCur = Tmp;
	}
	if (pCur) 
	{//清空最后一个节点
		free(pCur);
		pCur = NULL;
	}
}

/* 销毁链表 */
void list_destroy(node_t* s) 
{            
	assert(s);
	if (s->phead == NULL) 
	{
		free(s->phead);
		return;
	}
	while (s->phead) 
	{    
		plist_t Tmp = s->phead->next;
		free(s->phead);
		s->phead = Tmp;
	}
}

/* 打印链表 */
void list_print(node_t* s) 
{             
	uint i = 0;
	assert(s);
	plist_t pCur = s->phead;
	while (pCur) 
	{
		printf("node%d=%d --> ",i, pCur->data);
		pCur = pCur->next;
		i++;
	}
	printf("\n");
}


void main() 
{
	node_t s;
	list_init(&s);
	list_tail_insert_node(&s, 1);
	list_tail_insert_node(&s, 2);
	list_tail_insert_node(&s, 3);
	printf("size=%d\n", get_list_node_cnt(&s));
	list_print(&s);
	list_insert_node_via_position(list_find_node(&s, 2), 0);
	list_print(&s);
	list_remove_data(&s, 2);
	list_print(&s);
	#if 0 /* test for assert function */
	node_t *ps = NULL;
	list_init(ps);
	#endif
	return;
}


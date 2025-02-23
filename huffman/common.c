#include "common.h"

//比大小，判断节点的先后顺序，频度优先，频度相同字节值更大优先
//输入：两个节点指针
//输出：1 a<b 0 a>b
char NodeCompare(ListNode* a, ListNode* b)
{
	if (a->frequency < b->frequency)		//频度小
		return 1;
	else if (a->frequency == b->frequency && a->c < b->c)		//频度相同，字节值小
		return 1;
	else
		return 0;
}

//合并节点，频度为两节点频度之和，字节值为最大的字节值
//输入：两个节点
//输出：新节点指针
ListNode* CreatNode(ListNode* a, ListNode* b)
{
	ListNode* new = (ListNode*)malloc(sizeof(ListNode));
	new->c = a->c > b->c ? a->c : b->c;
	new->frequency = a->frequency + b->frequency;
	new->code = (unsigned char*)malloc(3 * sizeof(unsigned char));
	new->len = 0;
	new->parent = NULL;
	new->left = a;
	new->right = b;
	new->next = NULL;
	return new;
}
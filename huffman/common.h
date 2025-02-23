#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//结点结构，哈夫曼树,频度链表,堆排序共用
typedef struct ListNode
{
	unsigned char c;		    //结点的字节值
	int frequency;               //词频

	unsigned char* code;                 //字符的编码(对哈夫曼树结点有效)
	char len;					//编码长度
	struct ListNode* parent;       //结点的双亲结点(对哈夫曼树结点有效)
	struct ListNode* left;         //结点的左子树(对哈夫曼树结点有效)
	struct ListNode* right;        //结点的右子树(对哈夫曼树结点有效)

	struct ListNode* next;        //结点的后继结点(对频度链表结点有效)
}ListNode, HuffmanTree;

char NodeCompare(ListNode* a, ListNode* b);
ListNode* CreatNode(ListNode* a, ListNode* b);
#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//���ṹ����������,Ƶ������,��������
typedef struct ListNode
{
	unsigned char c;		    //�����ֽ�ֵ
	int frequency;               //��Ƶ

	unsigned char* code;                 //�ַ��ı���(�Թ������������Ч)
	char len;					//���볤��
	struct ListNode* parent;       //����˫�׽��(�Թ������������Ч)
	struct ListNode* left;         //����������(�Թ������������Ч)
	struct ListNode* right;        //����������(�Թ������������Ч)

	struct ListNode* next;        //���ĺ�̽��(��Ƶ����������Ч)
}ListNode, HuffmanTree;

char NodeCompare(ListNode* a, ListNode* b);
ListNode* CreatNode(ListNode* a, ListNode* b);
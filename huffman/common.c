#include "common.h"

//�ȴ�С���жϽڵ���Ⱥ�˳��Ƶ�����ȣ�Ƶ����ͬ�ֽ�ֵ��������
//���룺�����ڵ�ָ��
//�����1 a<b 0 a>b
char NodeCompare(ListNode* a, ListNode* b)
{
	if (a->frequency < b->frequency)		//Ƶ��С
		return 1;
	else if (a->frequency == b->frequency && a->c < b->c)		//Ƶ����ͬ���ֽ�ֵС
		return 1;
	else
		return 0;
}

//�ϲ��ڵ㣬Ƶ��Ϊ���ڵ�Ƶ��֮�ͣ��ֽ�ֵΪ�����ֽ�ֵ
//���룺�����ڵ�
//������½ڵ�ָ��
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
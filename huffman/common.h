#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//���ṹ����������,Ƶ������,��������
typedef struct ListNode
{
	unsigned char c;		    //�����ֽ�ֵ
	int frequency;               //��Ƶ

	char* code;                 //�ַ��ı���(�Թ������������Ч)
	struct ListNode* parent;       //����˫�׽��(�Թ������������Ч)
	struct ListNode* left;         //����������(�Թ������������Ч)
	struct ListNode* right;        //����������(�Թ������������Ч)

	struct ListNode* sleft;			//����������(�Զ���������Ч)
	struct ListNode* sright;		//����������(�Զ���������Ч)

	struct ListNode* next;        //���ĺ�̽��(��Ƶ����������Ч)
}ListNode, HuffmanTree;


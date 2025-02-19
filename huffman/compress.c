#include "common.h"

int compress(char* file_name);
int Count(FILE* f, ListNode** list);
char IfExist(ListNode** list, int num, unsigned char cur);
void HeapAdjust(ListNode** list, int s, int m);
char NodeCompare(ListNode* a, ListNode* b);
ListNode* CreatNode(ListNode* a, ListNode* b);

//ѹ������
//���룺�ļ������շ�������ѧ��
//�����xxx.hfm ��������code.txt �����
//����״̬�룺-1 �ļ���ʧ�� 0 ��������
int compress(char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	if (fp == NULL) return -1;
	ListNode** list = (ListNode**)malloc(sizeof(ListNode*) * 257);		//�Ѳ���˳�����
	if (list == NULL) return -2;
	int num = Count(fp, list);		//Ԥ�����ļ�
	
	ListNode* min1, * min2;		//��С����С�ڵ�
	for (int j = num / 2; j > 0; j--)		//��ʼ��������С����
		HeapAdjust(list, j, num);
	ListNode* head = (ListNode*)malloc(sizeof(ListNode));
	ListNode* p = head;
	int n = num;
	while (n >= 2)		//�������뽨��������
	{
		min1 = list[1];
		if (!min1->left && !min1->right)
		{
			p->next = min1;
			p = p->next;
			printf("0x%02x\t%d\n", p->c, p->frequency);
		}
		list[1] = list[n];
		n--;
		HeapAdjust(list, 1, n);
		min2 = list[1];
		if (!min2->left && !min2->right)
		{
			p->next = min2;
			p = p->next;
			printf("0x%02x\t%d\n", p->c, p->frequency);
		}
		list[1] = CreatNode(min1, min2);
		min1->parent = list[1];
		min2->parent = list[1];
		HeapAdjust(list, 1, n);
	}
	HuffmanTree* tree = list[1];
	//p = head->next;
	//while (p->next) {
	//	if(!p->left && !p->right)
	//		printf("%c\t0x%x\t%d\n", p->c, p->c, p->frequency);
	//	p = p->next;
	//}

	//free(list);
	fclose(fp);
	return 0;
}

//�ļ�����ͳ�Ʋ�ͬ�ֽ�ֵ������Ƶ��
//���룺�ļ�ָ�룬�ڵ�˳���������Ϣ
//������ڵ����
int Count(FILE*f, ListNode** list)
{
	int num = 0;	//�ڵ����

	int cur;
	while ((cur = fgetc(f)) != EOF)		//���ֽڶ�ȡ�ļ�
	{
		if (!IfExist(list, num, (unsigned char)cur))		//�����ڣ��½��ڵ�
		{
			num++;
			ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
			temp->c = (unsigned char)cur;
			temp->frequency = 1;
			temp->code = NULL;
			temp->len = 0;
			temp->parent = NULL;
			temp->left = NULL;
			temp->right = NULL;
			temp->next = NULL;
			list[num] = temp;		//����ڵ�˳���
		}
	}
	return num;
}

//�ظ��жϣ���ǰ�ֽ�ֵ�Ƿ����
//���룺�ڵ�˳����ڵ�������ǰ�ֽ�ֵ
//�����1 ���� 0 ������
char IfExist(ListNode** list, int num, unsigned char cur)
{
	for (int i = 0; i < num; i++)
	{
		if (list[i + 1]->c == cur)		//����
		{
			list[i + 1]->frequency++;
			return 1;
		}
	}
	return 0;		//������
}

//������ɸѡ����ʣ��Ԫ�ص���Ϊһ����
//���룺�ڵ�˳����Ѷ��ڵ���ţ��ڵ����
//�����С����
void HeapAdjust(ListNode** list, int s, int m)
{
	ListNode* temp = list[s];
	for (int i = 2 * s; i <= m; i *= 2)		//iΪ�������Ѷ��ĺ��ӽڵ�
	{
		if (i < m && !NodeCompare(list[i], list[i + 1])) i++;		//i+1�ڵ�С��i�ڵ㣬����iΪ��С���ӽڵ�
		if (NodeCompare(temp, list[i])) break;		//�Ѷ��ڵ���С���������
		list[s] = list[i];		//���С���ӽڵ㽻��
		s = i;
	}
	list[s] = temp;
}

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
	new->code = NULL;
	new->len = 0;
	new->parent = NULL;
	new->left = a;
	new->right = b;
	new->next = NULL;
	return new;
}
#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct ListNode           //���ṹ������������Ƶ��������
{
	char c;                     //�����ַ�
	int frequency;               //�ַ���Ƶ��
	char* code;                 //�ַ��ı���(�Թ������������Ч)
	struct ListNode* parent;       //����˫�׽��(�Թ������������Ч)
	struct ListNode* left;         //����������(�Թ������������Ч)
	struct ListNode* right;        //����������(�Թ������������Ч)
	struct ListNode* next;        //���ĺ�̽��(��Ƶ����������Ч)
}ListNode, HuffmanTree;

void Count(ListNode* head);
int IfExist(ListNode* head, char cur);
void Sort(ListNode* head);
void Print(ListNode* head);
void CreatHT(ListNode* head);
void Insert(ListNode* head, HuffmanTree* Node);
int IfFinished(ListNode* head);
void HuffmanCode(ListNode* head);
void Code(HuffmanTree* root, char* s, int n);

int WPL = 0;   //��Ȩ·������
int main() {
	char name[50];
	scanf("%s", name);
	FILE* fp = fopen(name, "r");
	if (fp == NULL) {
		perror("���ļ�ʧ��");
		return -1;
	}
	// д�ļ�����
	ListNode* head = (ListNode*)malloc(sizeof(ListNode));
	Count(head);
	Sort(head->next);
	CreatHT(head);
	HuffmanCode(head);
	Print(head->next);

	fclose(fp);
	return 0;
}

//����������
void HuffmanCode(ListNode* head) {
	char s[15] = { 0 };
	Code(head->next, s, 0);
}

void Code(HuffmanTree* root, char* s, int n) {
	if (root->right)    //������
	{
		*(s + n) = '1';
		Code(root->right, s, n + 1);
	}
	if (root->left)     //������
	{
		*(s + n) = '0';
		Code(root->left, s, n + 1);
	}
	if (!root->left && !root->right)    //Ҷ�ڵ�
	{
		*(s + n) = '\0';
		root->code = (char*)malloc(10 * sizeof(char));
		strcpy(root->code, s);
		WPL += root->frequency * n;
	}
}

//������������
void CreatHT(ListNode* head) {
	while (!IfFinished(head->next))     //�жϹ��������Ƿ������
	{
		ListNode* HeadNode = head;
		HuffmanTree* min1 = HeadNode, * min2 = HeadNode;
		for (ListNode* p = HeadNode; p; p = p->next)  //ѡ��Ƶ����С������
		{
			if (!p->parent)    //��˫�׽ڵ㼰δ���������������
			{
				min1 = min2;
				min2 = p;
			}
		}
		HuffmanTree* ParentNode = (HuffmanTree*)malloc(sizeof(HuffmanTree));
		ParentNode->left = min1;
		ParentNode->right = min2;
		ParentNode->frequency = min1->frequency + min2->frequency;
		ParentNode->parent = NULL;
		ParentNode->code = NULL;
		min1->parent = ParentNode;
		min2->parent = ParentNode;
		Insert(head, ParentNode);
	}
}

//���ڵ㰴Ƶ�ȴ�С����Ƶ������
void Insert(ListNode* head, HuffmanTree* Node) {
	ListNode* p = head->next;
	if (p->frequency <= Node->frequency) {
		head->next = Node;
		Node->next = p;
	}
	else {
		while (p->next->frequency > Node->frequency) p = p->next;
		Node->next = p->next;
		p->next = Node;
	}
}

//�жϹ��������Ƿ������
int IfFinished(ListNode* head) {
	for (ListNode* p = head->next; p; p = p->next) //�ж�����ڵ��Ƿ�ȫ�и��ڵ�

		if (!p->parent) return 0;
	return 1;
}

//����Ƶ������
void Count(ListNode* head) {
	char cur;
	ListNode* p = (ListNode*)malloc(sizeof(ListNode));
	head->next = p;
	p->c = getchar();
	p->frequency = 1;
	p->next = NULL;
	p->parent = NULL;
	p->left = NULL;
	p->right = NULL;
	while ((cur = getchar()) != EOF) {
		if (!IfExist(head->next, cur)) {
			ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
			temp->c = cur;
			temp->frequency = 1;
			temp->next = NULL;
			temp->parent = NULL;
			temp->left = NULL;
			temp->right = NULL;
			p->next = temp;
			p = temp;
		}
	}
}

//�ж�������Ϊc�Ľڵ��Ƿ����
int IfExist(ListNode* head, char cur) {
	for (ListNode* temp = head; temp != NULL; temp = temp->next) {
		if (temp->c == cur) {
			temp->frequency++;
			return 1;
		}
	}
	return 0;
}

//�Ը��ڵ�������Ӵ�С����
void Sort(ListNode* head) {
	for (ListNode* p = head; p->next; p = p->next) {
		for (ListNode* q = head; q->next; q = q->next) {
			if (q->next->frequency > q->frequency) {
				char temp_c = q->c;
				q->c = q->next->c;
				q->next->c = temp_c;
				int temp_f = q->frequency;
				q->frequency = q->next->frequency;
				q->next->frequency = temp_f;
			}
		}
	}
}

//�������
void Print(ListNode* head) {
	for (ListNode* p = head; p; p = p->next) {
		if (p->code) {
			if (p->c != '\n') printf("'%c' %d %s\n", p->c, p->frequency, p->code);
			else printf("'\\n' %d %s\n", p->frequency, p->code);
		}
	}
	printf("%d", WPL);
}

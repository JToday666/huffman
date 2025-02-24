#include "common.h"
#include "hash.h"

int compress(char* file_name);
int Count(FILE* f, ListNode** list);
char IfExist(ListNode** list, int num, unsigned char cur);
void HeapAdjust(ListNode** list, int s, int m);
int HuffmanCode(HuffmanTree* root);
void code_txt(ListNode* head, int size);
void encoding_hfm(char* name, FILE* fp, ListNode* head);

//ѹ������
//���룺�ļ������շ�������ѧ��
//�����xxx.hfm ��������code.txt �����
//����״̬�룺-1 �ļ���ʧ�� 0 �������� -2 �ڴ���䲻��
int compress(char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	if (fp == NULL) return -1;
	ListNode** list = (ListNode**)malloc(sizeof(ListNode*) * 257);		//�Ѳ���˳�����
	if (list == NULL) return -2;
	int size = Count(fp, list);		//Ԥ�����ļ�
	int num = list[0]->frequency;

	printf("*****Ƶ�ʱ�*****\nbyte\tfreq\n");
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
	tree->code[0] = 0;
	tree->code[1] = 0;
	tree->code[2] = 0;

	int WPL = HuffmanCode(tree);		//����������
	printf("*****�����*****\nbyte len  code\n");
	code_txt(head, size);		//��������code.txt
	printf("WPL: %d\n", WPL);
	printf("size: %d\n", size);
	unsigned long long hash_txt = fnv1a_64(file_name);		//xxx.txt��ϣ
	encoding_hfm(strtok(file_name, "."), fp, head);		//ѹ�������xxx.hfm
	unsigned long long hash_hfm = fnv1a_64(file_name);		//xxx.hfm��ϣ

	//free(list);
	fclose(fp);
	return 0;
}

//�ļ�����ͳ�Ʋ�ͬ�ֽ�ֵ������Ƶ��
//���룺�ļ�ָ�룬�ڵ�˳���������Ϣ
//������ļ��ֽ�����ͨ��list[0]->frequency���ؽڵ����
int Count(FILE*f, ListNode** list)
{
	int size = 0;		//�ļ��ֽ���
	int num = 0;	//�ڵ����

	int cur;
	while ((cur = fgetc(f)) != EOF)		//���ֽڶ�ȡ�ļ�
	{
		size++;
		if (!IfExist(list, num, (unsigned char)cur))		//�����ڣ��½��ڵ�
		{
			num++;
			ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
			temp->c = (unsigned char)cur;
			temp->frequency = 1;
			temp->code = (unsigned char*)malloc(3 * sizeof(unsigned char));
			temp->len = 0;
			temp->parent = NULL;
			temp->left = NULL;
			temp->right = NULL;
			temp->next = NULL;
			list[num] = temp;		//����ڵ�˳���
		}
	}
	ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
	temp->frequency = num;
	list[0] = temp;
	rewind(f);
	return size;
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

//���������룬ȷ��code��len
//���룺����������������ڵ�
//�����WPLֵ
int HuffmanCode(HuffmanTree* root)
{
	if (!root->left)		//Ҷ�ڵ�
		return root->frequency * root->len;
	else
	{
		//������
		root->left->len = root->len + 1;
		for (char i = 0; i < 3; i++) root->left->code[i] = root->code[i];
		int Lwpl = HuffmanCode(root->left);
		//������
		root->right->len = root->len + 1;
		for (char i = 0; i < 3; i++) root->right->code[i] = root->code[i];
		char byt = root->len / 8;
		char loc = root->len % 8 + 1;
		root->right->code[byt] |= 0x80 >> (loc - 1);
		int Rwpl = HuffmanCode(root->right);
		return Lwpl + Rwpl;
	}
}

//������ļ�
//���룺�ڵ�����, ԭʼ�ļ���С
//�����code.txt�ļ�
void code_txt(ListNode* head, int size)
{
	FILE* fp = fopen("code.txt", "w");
	if (fp == NULL) exit(-1);

	fprintf(fp, "%d", size);		//���ֽ���
	for (ListNode* p = head->next; p; p = p->next)
	{
		fprintf(fp, "\n0x%02x 0x%02x", p->c, p->len);		//�ֽ��� ���볤��
		printf("0x%02x 0x%02x", p->c, p->len);
		for (char i = 0; i <= (p->len - 1) / 8; i++)
		{
			fprintf(fp, " 0x%02x", p->code[i]);		//λ�����ֽ�(β����0)
			printf(" 0x%02x", p->code[i]);
		}
		printf("\n");
	}
	fclose(fp);
}

//ѹ���ļ�
//���룺ԭ�ļ�����ԭ�ļ�ָ�룬�ڵ�����
//�����xxx.hfm�ļ�
void encoding_hfm(char* name, FILE* fp, ListNode* head)
{
	FILE* f = fopen(strcat(name, ".hfm"), "wb+");
	int byt = 0;		//ѹ�����С
	char loc = 0;		//��ǰλ
	unsigned char buf = 0;		//�ֽڱ༭
	int cur;
	while ((cur = fgetc(fp)) != EOF)
	{
		ListNode* p;
		for (p = head->next; p; p = p->next)
			if (p->c == (unsigned char)cur) break;		//��Ӧ�ڵ�
		for (char i = 0; i < p->len; i++)
		{
			if ((p->code[i / 8] << (i % 8)) & 0x80)		//�����������λΪ1
				buf |= 0x80 >> loc;
			loc++;
			if (loc == 8)		//����
			{
				fputc(buf, f);
				buf = 0;
				loc = 0;
				byt++;
			}
		}
	}
	if (loc)
	{
		fputc(buf, f);
		byt++;
	}
	fseek(f, -16, SEEK_END);
	printf("last 16 byte:\n");
	for (int j = 0; j < 16; j++) printf("0x%02x ", (unsigned char)fgetc(f));
	printf("\n");
	fclose(f);
}
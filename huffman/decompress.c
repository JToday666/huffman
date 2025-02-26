#include "common.h"
#include "hash.h"
#include <time.h>
#include <locale.h>

int decompress(char* file_name, int encode, char* sender, char* reciever);
int build_tree(HuffmanTree* tree, int encode);
void decode(char* file_name, HuffmanTree* tree, int size);
int decode_massage(char* file_name, HuffmanTree* tree, char* sender, char* reciever);
void utf8_print(char* s);

//��ѹ��
//���룺�ļ���
//�����xxx_j.txt��ѹ���ļ�
//����ֵ
int decompress(char* file_name, int encode, char* sender, char* reciever)
{
	HuffmanTree* tree = (HuffmanTree*)malloc(sizeof(HuffmanTree));
	tree->left = NULL;
	tree->right = NULL;
	
	clock_t start = clock();
	int size = build_tree(tree, encode);		//��code.txt��ԭ�ɹ�������

	if (!decode_massage(file_name, tree, sender, reciever))
	{
		printf("��ݴ���");
		return -1;
	}
	decode(file_name, tree, size);		//��ѹ��
	clock_t end = clock();

	printf("**********�ļ�����**********\n");
	char* old_locale = setlocale(LC_ALL, NULL);
	if (old_locale) {
		old_locale = _strdup(old_locale);
	}
	// ������������Ϊ UTF-8
	setlocale(LC_ALL, "en_US.UTF-8");

	// �򿪽�ѹ������ļ�����ʾ����
	FILE* result = fopen(file_name, "r");
	if (result == NULL) {
		printf("�޷��򿪽�ѹ������ļ�\n");
		return;
	}
	printf("��ѹ������ļ�����:\n");
	char line[256];
	while (fgets(line, sizeof(line), result)) {
		printf("%s", line);
	}
	fclose(result);

	// �ָ�ԭ������������
	if (old_locale) {
		setlocale(LC_ALL, old_locale);
		free(old_locale);
	}
	printf("\n****************************\n");

	printf("Running Time��%dms\n", end - start);
	fnv1a_64(file_name);		//xxx_j.txt��ϣ
	return 0;
}

//����
//���룺���ڵ㣬����code.txt�ļ�
//������ֽڴ�С
int build_tree(HuffmanTree* tree, int encode)
{
	FILE* f = fopen("code.txt", "rb");
	int size = 0;
	unsigned char offset = 0;
	if (encode == 1)offset = 0x55;

	fscanf(f, "%d\n", &size);
	while (1)
	{
		unsigned char c = 0;		//�ֽ�ֵ
		char len = 0;		//���볤��
		int res = fscanf(f, "0x%hhx 0x%hhx", &c, &len);
		if (res == -1) break;		//�ļ�ĩβ
		unsigned char s[3] = { 0 };
		for (int i = 0; i <= (len - 1) / 8; i++)
			fscanf(f, " 0x%hhx", &s[i]);
		fscanf(f, "\n");
		HuffmanTree* p = tree;
		char byte = 0, loc = 0;
		while (len--)
		{
			if (0x80 >> loc & s[byte])		//������
			{
				if (p->right == NULL) 
				{ 
					p->right = (HuffmanTree*)malloc(sizeof(HuffmanTree)); 
					p->right->left = NULL;
					p->right->right = NULL;
				}
				p = p->right;
			}
			else		//������
			{
				if (p->left == NULL)
				{
					p->left = (HuffmanTree*)malloc(sizeof(HuffmanTree));
					p->left->left = NULL;
					p->left->right = NULL;
				}
				p = p->left;
			}
			if (++loc == 8)
			{
				loc = 0;
				byte++;
			}
		}
		//Ҷ�ڵ�
		if (encode == 2) p->c = ~c;
		else p->c = c - offset;
		p->len = len;
		p->left = NULL;
		p->right = NULL;
	}
	return size;
}

//����
//���룺ѹ���ļ������ڵ������ڵ㣬ԭʼ�ļ���С
//�����xxx_j.txt��ѹ���ļ�
void decode(char* file_name, HuffmanTree* tree, int size)
{
	FILE* fp = fopen(file_name, "rb");
	FILE* f = fopen(strcat(strtok(file_name, "."), "_j.txt"), "wb");
	unsigned char cur = 0;
	HuffmanTree* p = tree;
	
	while ((cur = fgetc(fp)) != EOF)
	{
		if (!size) break;		//��������
		for (char loc = 0; loc < 8; loc++)
		{
			if (0x80 >> loc & cur) p = p->right;		//��
			else p = p->left;		//��

			if (p->left == NULL)		//����Ҷ�ڵ�
			{
				fwrite(&(p->c), 1, 1, f);
				if (!(--size)) break;		//��������
				p = tree;
			}
		}
	}
	fclose(f);
	fclose(fp);
}

//�˶������Ϣ
int decode_massage(char* file_name, HuffmanTree* tree, char* sender, char* reciever)
{
	if (sender == NULL && reciever == NULL) return 1;

	FILE* fp = fopen(file_name, "rb");
	unsigned char cur = 0;
	HuffmanTree* p = tree;

	char send[50];
	char recieve[50];
	char* present = send;
	int n = 2;

	while (n)
	{
		cur = fgetc(fp);
		for (char loc = 0; loc < 8; loc++)
		{
			if (0x80 >> loc & cur) p = p->right;		//��
			else p = p->left;		//��

			if (p->left == NULL)		//����Ҷ�ڵ�
			{
				if (p->c == (unsigned char)'\n')
				{
					n--;
					*present = '\0';
					if (!n) break;
					present = recieve;
				}
				else
					*(present++) = p->c;
				p = tree;
			}
		}
	}
	rewind(fp);
	fclose(fp);

	printf("�����ˣ�");
	utf8_print(send);
	printf("�����ˣ�");
	utf8_print(recieve);

	if (strcmp(recieve, reciever)) return 0;
	return 1;
}

//utf-8��ʾ
void utf8_print(char* s)
{
	char* old_locale = setlocale(LC_ALL, NULL);
	if (old_locale) {
		old_locale = _strdup(old_locale);
	}
	// ������������Ϊ UTF-8
	setlocale(LC_ALL, "en_US.UTF-8");

	printf("%s\n", s);

	// �ָ�ԭ������������
	if (old_locale) {
		setlocale(LC_ALL, old_locale);
		free(old_locale);
	}
}
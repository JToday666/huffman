#include "common.h"
#include "hash.h"
#include <time.h>

int decompress(char* file_name);
int build_tree(HuffmanTree* tree);
void decode(char* file_name, HuffmanTree* tree, int size);

//��ѹ��
//���룺�ļ���
//�����xxx_j.txt��ѹ���ļ�
//����ֵ
int decompress(char* file_name)
{
	HuffmanTree* tree = (HuffmanTree*)malloc(sizeof(HuffmanTree));
	tree->left = NULL;
	tree->right = NULL;
	
	clock_t start = clock();
	int size = build_tree(tree);		//��code.txt��ԭ�ɹ�������
	decode(file_name, tree, size);		//��ѹ��
	clock_t end = clock();
	printf("Running Time��%dms\n", end - start);
	fnv1a_64(file_name);		//xxx_j.txt��ϣ
	return 0;
}

//����
//���룺���ڵ㣬����code.txt�ļ�
//������ֽڴ�С
int build_tree(HuffmanTree* tree)
{
	FILE* f = fopen("code.txt", "rb");
	int size = 0;
	fscanf(f, "%d\n", &size);
	unsigned char x = 0;
	while (1)
	{
		unsigned char c = 0;
		char len = 0;
		int res = fscanf(f, "0x%hhx 0x%hhx", &c, &len);
		if (res == -1) break;
		unsigned char s[3] = { 0 };
		for (int i = 0; i <= (len - 1) / 8; i++)
			fscanf(f, " 0x%hhx", &s[i]);
		fscanf(f, "\n");
		HuffmanTree* p = tree;
		char byte = 0, loc = 0;
		while (len--)
		{
			if (0x80 >> loc & s[byte])
			{
				if (p->right == NULL) 
				{ 
					p->right = (HuffmanTree*)malloc(sizeof(HuffmanTree)); 
					p->right->left = NULL;
					p->right->right = NULL;
				}
				p = p->right;
			}
			else
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
		p->c = c;
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
		if (!size) break;
		for (char loc = 0; loc < 8; loc++)
		{
			if (0x80 >> loc & cur) p = p->right;
			else p = p->left;

			if (p->left == NULL)
			{
				//fputc(p->c, f);
				fwrite(&(p->c), 1, 1, f);
				if (!(--size)) break;
				p = tree;
			}
		}
	}
	fclose(f);
	fclose(fp);
}
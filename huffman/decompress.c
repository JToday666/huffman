#include "common.h"

int decompress(char* file_name);
int build_tree(HuffmanTree* tree);
char add_node(FILE* f);

//��ѹ��
//���룺�ļ���
//�����xxx_j.txt��ѹ���ļ�
//����ֵ
int decompress(char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	HuffmanTree* tree = (HuffmanTree*)malloc(sizeof(HuffmanTree));
	tree->left = NULL;
	tree->right = NULL;

	int size = build_tree(tree);
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
	printf("%d\n", size);
	unsigned char x = 0;
	while (1)
	{
		unsigned char byt = 0, len = 0;
		int res = fscanf(f, "0x%hhx 0x%hhx", &byt, &len);
		if (res == -1) break;
		printf("%x\t%x\t", byt, len);
		unsigned char s[3] = { 0 };
		for (int i = 0; i <= (len - 1) / 8; i++)
		{
			fscanf(f, " 0x%hhx", &s[i]);
			printf(" ..0x%x.. ", s[i]);
		}
		fscanf(f, "\n");
		printf("\n");
	}
	//add_node(f);

	return size;
}

char add_node(FILE* f)
{

}
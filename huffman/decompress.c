#include "common.h"
#include "hash.h"
#include <time.h>
#include <locale.h>

int decompress(char* file_name, int encode, char* sender, char* reciever);
int build_tree(HuffmanTree* tree, int encode);
void decode(char* file_name, HuffmanTree* tree, int size);
int decode_massage(char* file_name, HuffmanTree* tree, char* sender, char* reciever);
void utf8_print(char* s);

//解压缩
//输入：文件名
//输出：xxx_j.txt解压缩文件
//返回值
int decompress(char* file_name, int encode, char* sender, char* reciever)
{
	HuffmanTree* tree = (HuffmanTree*)malloc(sizeof(HuffmanTree));
	tree->left = NULL;
	tree->right = NULL;
	
	clock_t start = clock();
	int size = build_tree(tree, encode);		//将code.txt还原成哈夫曼树

	if (!decode_massage(file_name, tree, sender, reciever))
	{
		printf("身份错误！");
		return -1;
	}
	decode(file_name, tree, size);		//解压缩
	clock_t end = clock();

	printf("**********文件内容**********\n");
	char* old_locale = setlocale(LC_ALL, NULL);
	if (old_locale) {
		old_locale = _strdup(old_locale);
	}
	// 设置区域设置为 UTF-8
	setlocale(LC_ALL, "en_US.UTF-8");

	// 打开解压缩后的文件并显示内容
	FILE* result = fopen(file_name, "r");
	if (result == NULL) {
		printf("无法打开解压缩后的文件\n");
		return;
	}
	printf("解压缩后的文件内容:\n");
	char line[256];
	while (fgets(line, sizeof(line), result)) {
		printf("%s", line);
	}
	fclose(result);

	// 恢复原来的区域设置
	if (old_locale) {
		setlocale(LC_ALL, old_locale);
		free(old_locale);
	}
	printf("\n****************************\n");

	printf("Running Time：%dms\n", end - start);
	fnv1a_64(file_name);		//xxx_j.txt哈希
	return 0;
}

//建树
//输入：根节点，读入code.txt文件
//输出：字节大小
int build_tree(HuffmanTree* tree, int encode)
{
	FILE* f = fopen("code.txt", "rb");
	int size = 0;
	unsigned char offset = 0;
	if (encode == 1)offset = 0x55;

	fscanf(f, "%d\n", &size);
	while (1)
	{
		unsigned char c = 0;		//字节值
		char len = 0;		//编码长度
		int res = fscanf(f, "0x%hhx 0x%hhx", &c, &len);
		if (res == -1) break;		//文件末尾
		unsigned char s[3] = { 0 };
		for (int i = 0; i <= (len - 1) / 8; i++)
			fscanf(f, " 0x%hhx", &s[i]);
		fscanf(f, "\n");
		HuffmanTree* p = tree;
		char byte = 0, loc = 0;
		while (len--)
		{
			if (0x80 >> loc & s[byte])		//右子树
			{
				if (p->right == NULL) 
				{ 
					p->right = (HuffmanTree*)malloc(sizeof(HuffmanTree)); 
					p->right->left = NULL;
					p->right->right = NULL;
				}
				p = p->right;
			}
			else		//左子树
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
		//叶节点
		if (encode == 2) p->c = ~c;
		else p->c = c - offset;
		p->len = len;
		p->left = NULL;
		p->right = NULL;
	}
	return size;
}

//解码
//输入：压缩文件名，节点树根节点，原始文件大小
//输出：xxx_j.txt解压缩文件
void decode(char* file_name, HuffmanTree* tree, int size)
{
	FILE* fp = fopen(file_name, "rb");
	FILE* f = fopen(strcat(strtok(file_name, "."), "_j.txt"), "wb");
	unsigned char cur = 0;
	HuffmanTree* p = tree;
	
	while ((cur = fgetc(fp)) != EOF)
	{
		if (!size) break;		//结束处理
		for (char loc = 0; loc < 8; loc++)
		{
			if (0x80 >> loc & cur) p = p->right;		//右
			else p = p->left;		//左

			if (p->left == NULL)		//到达叶节点
			{
				fwrite(&(p->c), 1, 1, f);
				if (!(--size)) break;		//结束处理
				p = tree;
			}
		}
	}
	fclose(f);
	fclose(fp);
}

//核对身份信息
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
			if (0x80 >> loc & cur) p = p->right;		//右
			else p = p->left;		//左

			if (p->left == NULL)		//到达叶节点
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

	printf("发送人：");
	utf8_print(send);
	printf("接收人：");
	utf8_print(recieve);

	if (strcmp(recieve, reciever)) return 0;
	return 1;
}

//utf-8显示
void utf8_print(char* s)
{
	char* old_locale = setlocale(LC_ALL, NULL);
	if (old_locale) {
		old_locale = _strdup(old_locale);
	}
	// 设置区域设置为 UTF-8
	setlocale(LC_ALL, "en_US.UTF-8");

	printf("%s\n", s);

	// 恢复原来的区域设置
	if (old_locale) {
		setlocale(LC_ALL, old_locale);
		free(old_locale);
	}
}
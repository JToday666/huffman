#include "common.h"
#include "hash.h"

int compress(char* file_name);
int Count(FILE* f, ListNode** list);
char IfExist(ListNode** list, int num, unsigned char cur);
void HeapAdjust(ListNode** list, int s, int m);
int HuffmanCode(HuffmanTree* root);
void code_txt(ListNode* head, int size);
void encoding_hfm(char* name, FILE* fp, ListNode* head);

//压缩处理
//输入：文件名，收发人姓名学号
//输出：xxx.hfm 编码结果，code.txt 编码表
//返回状态码：-1 文件打开失败 0 正常结束 -2 内存分配不足
int compress(char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	if (fp == NULL) return -1;
	ListNode** list = (ListNode**)malloc(sizeof(ListNode*) * 257);		//堆采用顺序表储存
	if (list == NULL) return -2;
	int size = Count(fp, list);		//预处理文件
	int num = list[0]->frequency;

	printf("*****频率表*****\nbyte\tfreq\n");
	ListNode* min1, * min2;		//最小，次小节点
	for (int j = num / 2; j > 0; j--)		//初始化，建成小顶堆
		HeapAdjust(list, j, num);
	ListNode* head = (ListNode*)malloc(sizeof(ListNode));
	ListNode* p = head;
	int n = num;
	while (n >= 2)		//堆排序与建哈夫曼树
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

	int WPL = HuffmanCode(tree);		//哈夫曼编码
	printf("*****编码表*****\nbyte len  code\n");
	code_txt(head, size);		//输出编码表code.txt
	printf("WPL: %d\n", WPL);
	printf("size: %d\n", size);
	unsigned long long hash_txt = fnv1a_64(file_name);		//xxx.txt哈希
	encoding_hfm(strtok(file_name, "."), fp, head);		//压缩，输出xxx.hfm
	unsigned long long hash_hfm = fnv1a_64(file_name);		//xxx.hfm哈希

	//free(list);
	fclose(fp);
	return 0;
}

//文件处理，统计不同字节值个数及频度
//输入：文件指针，节点顺序表，附加信息
//输出：文件字节数，通过list[0]->frequency返回节点个数
int Count(FILE*f, ListNode** list)
{
	int size = 0;		//文件字节数
	int num = 0;	//节点个数

	int cur;
	while ((cur = fgetc(f)) != EOF)		//按字节读取文件
	{
		size++;
		if (!IfExist(list, num, (unsigned char)cur))		//不存在，新建节点
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
			list[num] = temp;		//存入节点顺序表
		}
	}
	ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
	temp->frequency = num;
	list[0] = temp;
	rewind(f);
	return size;
}

//重复判断，当前字节值是否存在
//输入：节点顺序表，节点数，当前字节值
//输出：1 存在 0 不存在
char IfExist(ListNode** list, int num, unsigned char cur)
{
	for (int i = 0; i < num; i++)
	{
		if (list[i + 1]->c == cur)		//存在
		{
			list[i + 1]->frequency++;
			return 1;
		}
	}
	return 0;		//不存在
}

//堆排序筛选，将剩余元素调整为一个堆
//输入：节点顺序表，堆顶节点序号，节点个数
//输出：小顶堆
void HeapAdjust(ListNode** list, int s, int m)
{
	ListNode* temp = list[s];
	for (int i = 2 * s; i <= m; i *= 2)		//i为待调整堆顶的孩子节点
	{
		if (i < m && !NodeCompare(list[i], list[i + 1])) i++;		//i+1节点小于i节点，调整i为较小孩子节点
		if (NodeCompare(temp, list[i])) break;		//堆顶节点最小，调整完毕
		list[s] = list[i];		//与较小孩子节点交换
		s = i;
	}
	list[s] = temp;
}

//哈夫曼编码，确定code与len
//输入：待编码哈夫曼树根节点
//输出：WPL值
int HuffmanCode(HuffmanTree* root)
{
	if (!root->left)		//叶节点
		return root->frequency * root->len;
	else
	{
		//左子树
		root->left->len = root->len + 1;
		for (char i = 0; i < 3; i++) root->left->code[i] = root->code[i];
		int Lwpl = HuffmanCode(root->left);
		//右子树
		root->right->len = root->len + 1;
		for (char i = 0; i < 3; i++) root->right->code[i] = root->code[i];
		char byt = root->len / 8;
		char loc = root->len % 8 + 1;
		root->right->code[byt] |= 0x80 >> (loc - 1);
		int Rwpl = HuffmanCode(root->right);
		return Lwpl + Rwpl;
	}
}

//编码表文件
//输入：节点链表, 原始文件大小
//输出：code.txt文件
void code_txt(ListNode* head, int size)
{
	FILE* fp = fopen("code.txt", "w");
	if (fp == NULL) exit(-1);

	fprintf(fp, "%d", size);		//总字节数
	for (ListNode* p = head->next; p; p = p->next)
	{
		fprintf(fp, "\n0x%02x 0x%02x", p->c, p->len);		//字节码 编码长度
		printf("0x%02x 0x%02x", p->c, p->len);
		for (char i = 0; i <= (p->len - 1) / 8; i++)
		{
			fprintf(fp, " 0x%02x", p->code[i]);		//位编码字节(尾部补0)
			printf(" 0x%02x", p->code[i]);
		}
		printf("\n");
	}
	fclose(fp);
}

//压缩文件
//输入：原文件名，原文件指针，节点链表
//输出：xxx.hfm文件
void encoding_hfm(char* name, FILE* fp, ListNode* head)
{
	FILE* f = fopen(strcat(name, ".hfm"), "wb+");
	int byt = 0;		//压缩后大小
	char loc = 0;		//当前位
	unsigned char buf = 0;		//字节编辑
	int cur;
	while ((cur = fgetc(fp)) != EOF)
	{
		ListNode* p;
		for (p = head->next; p; p = p->next)
			if (p->c == (unsigned char)cur) break;		//对应节点
		for (char i = 0; i < p->len; i++)
		{
			if ((p->code[i / 8] << (i % 8)) & 0x80)		//哈夫曼编码该位为1
				buf |= 0x80 >> loc;
			loc++;
			if (loc == 8)		//填满
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
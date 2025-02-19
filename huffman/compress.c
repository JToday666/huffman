#include "common.h"

int compress(char* file_name);
int Count(FILE* f, ListNode** list);
char IfExist(ListNode** list, int num, unsigned char cur);
void HeapAdjust(ListNode** list, int s, int m);
char NodeCompare(ListNode* a, ListNode* b);
ListNode* CreatNode(ListNode* a, ListNode* b);

//压缩处理
//输入：文件名，收发人姓名学号
//输出：xxx.hfm 编码结果，code.txt 编码表
//返回状态码：-1 文件打开失败 0 正常结束
int compress(char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	if (fp == NULL) return -1;
	ListNode** list = (ListNode**)malloc(sizeof(ListNode*) * 257);		//堆采用顺序表储存
	if (list == NULL) return -2;
	int num = Count(fp, list);		//预处理文件
	
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

//文件处理，统计不同字节值个数及频度
//输入：文件指针，节点顺序表，附加信息
//输出：节点个数
int Count(FILE*f, ListNode** list)
{
	int num = 0;	//节点个数

	int cur;
	while ((cur = fgetc(f)) != EOF)		//按字节读取文件
	{
		if (!IfExist(list, num, (unsigned char)cur))		//不存在，新建节点
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
			list[num] = temp;		//存入节点顺序表
		}
	}
	return num;
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

//比大小，判断节点的先后顺序，频度优先，频度相同字节值更大优先
//输入：两个节点指针
//输出：1 a<b 0 a>b
char NodeCompare(ListNode* a, ListNode* b)
{
	if (a->frequency < b->frequency)		//频度小
		return 1;
	else if (a->frequency == b->frequency && a->c < b->c)		//频度相同，字节值小
		return 1;
	else
		return 0;
}

//合并节点，频度为两节点频度之和，字节值为最大的字节值
//输入：两个节点
//输出：新节点指针
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
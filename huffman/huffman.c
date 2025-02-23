#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct ListNode           //结点结构，哈夫曼树与频度链表共用
{
	unsigned char c;		    //结点的字符
	int frequency;               //字符的频度
	char* code;                 //字符的编码(对哈夫曼树结点有效)
	struct ListNode* parent;       //结点的双亲结点(对哈夫曼树结点有效)
	struct ListNode* left;         //结点的左子树(对哈夫曼树结点有效)
	struct ListNode* right;        //结点的右子树(对哈夫曼树结点有效)
	struct ListNode* next;        //结点的后继结点(对频度链表结点有效)
}ListNode, HuffmanTree;

void Count(ListNode* head, FILE* f);
int IfExist(ListNode* head, char cur);
void Sort(ListNode* head);
void Print(ListNode* head);
void CreatHT(ListNode* head);
void Insert(ListNode* head, HuffmanTree* Node);
int IfFinished(ListNode* head);
void HuffmanCode(ListNode* head);
void Code(HuffmanTree* root, char* s, int n);

int WPL = 0;   //带权路径长度
int length = 0; //文本长度
int main() {
	char name[50];
	scanf("%s", name);
	FILE* fp = fopen(name, "r");
	if (fp == NULL) {
		perror("打开文件失败");
		return -1;
	}

	ListNode* head = (ListNode*)malloc(sizeof(ListNode));
	Count(head, fp);
	Sort(head->next);
	CreatHT(head);
	HuffmanCode(head);
	Print(head->next);

	fclose(fp);
	return 0;
}

//哈夫曼编码
void HuffmanCode(ListNode* head) {
	char s[15] = { 0 };
	Code(head->next, s, 0);
}

//递归
void Code(HuffmanTree* root, char* s, int n) {
	if (root->right)    //右子树
	{
		*(s + n) = '1';
		Code(root->right, s, n + 1);
	}
	if (root->left)     //左子树
	{
		*(s + n) = '0';
		Code(root->left, s, n + 1);
	}
	if (!root->left && !root->right)    //叶节点
	{
		*(s + n) = '\0';
		root->code = (char*)malloc(10 * sizeof(char));
		strcpy(root->code, s);
		WPL += root->frequency * n;
	}
}

//建立哈夫曼树
void CreatHT(ListNode* head) {
	while (!IfFinished(head->next))     //判断哈夫曼树是否建立完毕
	{
		ListNode* HeadNode = head->next;
		HuffmanTree* min1 = HeadNode, * min2 = HeadNode;
		for (ListNode* p = HeadNode; p; p = p->next)  //选出频度最小的两个
		{
			if (!p->parent) min1 = p;   //无双亲节点及未被加入哈夫曼树中
			min2 = p->next;
			break;
		}
		HuffmanTree* ParentNode = (HuffmanTree*)malloc(sizeof(HuffmanTree));
		ParentNode->left = min1;
		ParentNode->right = min2;
		ParentNode->frequency = min1->frequency + min2->frequency;
		ParentNode->c = min1->c > min2->c ? min1->c : min2->c;
		ParentNode->parent = NULL;
		ParentNode->code = NULL;
		min1->parent = ParentNode;
		min2->parent = ParentNode;
		Insert(head, ParentNode);
	}
}

//将节点按频度大小插入频度链表
void Insert(ListNode* head, HuffmanTree* Node) {

}

//判断哈夫曼树是否建立完毕
int IfFinished(ListNode* head) {
	for (ListNode* p = head->next; p; p = p->next) //判断链表节点是否全有父节点
		if (!p->parent) return 0;
	return 1;
}

//建立频度链表//
void Count(ListNode* head, FILE* f) {
	char cur;
	ListNode* p = (ListNode*)malloc(sizeof(ListNode));
	head->next = p;
	p->c = fgetc(f);
	length++;
	p->frequency = 1;
	p->next = NULL;
	p->parent = NULL;
	p->left = NULL;
	p->right = NULL;
	while ((cur = fgetc(f)) != EOF) {
		if (!IfExist(head->next, cur)) {
			length++;
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

//判断数据域为c的节点是否存在//
int IfExist(ListNode* head, char cur) {
	for (ListNode* temp = head; temp != NULL; temp = temp->next) {
		if (temp->c == cur) {
			temp->frequency++;
			return 1;
		}
	}
	return 0;
}

//对各节点数据域从小到大排序//
void Sort(ListNode* head) {

}

//输出编码
void Print(ListNode* head) {
	for (ListNode* p = head; p; p = p->next) {
		if (p->code) {
			if (p->c != '\n') printf("'%c' %d %s\n", p->c, p->frequency, p->code);
			else printf("'\\n' %d %s\n", p->frequency, p->code);
		}
	}
	printf("%d", WPL);
}

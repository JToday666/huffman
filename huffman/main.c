#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//extern void info(char* message);
extern void inquire_base(char* sender, char* reciever);
extern int compress(char* file_name, int encode, char* info);
extern int decompress(char* file_name, int encode, char* sender, char* reciever);

int main() {
	int op = 0;
	char file_name[20];

	printf("1:压缩\t\t2:解压缩\n");
	printf("输入操作:");
	scanf("%d", &op);
	if (op != 1 && op != 2) 
		printf("error");
	else {
		printf("输入文件名:");
		scanf("%s", file_name);
		int encode = 0, head = 0;
		printf("是否加密(0-不加密 1-偏移量 2-位取反):");
		scanf("%d", &encode);
		printf("是否增加收发人信息(0/1):");
		scanf("%d", &head);

		char* message = (char*)malloc(sizeof(char) * 100);
		char* sender = (char*)malloc(sizeof(char) * 50);
		char* reciever = (char*)malloc(sizeof(char) * 50);
		if (head == 1) 
		{
			inquire_base(sender, reciever);
			strcpy(message, sender);
			strcat(message, "\n");
			strcat(message, reciever);
			strcat(message, "\n");
		}
		else 
		{
			message = NULL;
			sender = NULL;
			reciever = NULL;
		}
		//printf("%d\n%s\n", encode, message);
		if (op == 1)
		{
			printf("压缩\n");
			int res = compress(file_name, encode, message);
			switch (res)
			{
			case 0:
				printf("finish");
				break;
			case -1:
				printf("file open error");
				break;
			default:
				printf("unnown error");
				break;
			}
		}
		else
		{
			printf("解压缩\n");
			decompress(file_name, encode, sender, reciever);
		}
	}
	return 0;
}
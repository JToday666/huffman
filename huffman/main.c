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

	printf("1:ѹ��\t\t2:��ѹ��\n");
	printf("�������:");
	scanf("%d", &op);
	if (op != 1 && op != 2) 
		printf("error");
	else {
		printf("�����ļ���:");
		scanf("%s", file_name);
		int encode = 0, head = 0;
		printf("�Ƿ����(0-������ 1-ƫ���� 2-λȡ��):");
		scanf("%d", &encode);
		printf("�Ƿ������շ�����Ϣ(0/1):");
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
			printf("ѹ��\n");
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
			printf("��ѹ��\n");
			decompress(file_name, encode, sender, reciever);
		}
	}
	return 0;
}
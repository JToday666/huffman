#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>

extern int compress(char* file_name);
int main() {
	int op = 0;
	char file_name[20];
	//char send_ID[20];
	//char send_name[20];
	//char recieve_ID[20];
	//char recieve_name[20];
	printf("1:ѹ��\t\t2:��ѹ��\n");
	printf("�������:");
	scanf("%d", &op);
	if (op != 1 && op != 2) 
		printf("error");
	else {
		printf("�����ļ���:");
		scanf("%s", file_name);
		//printf("���뷢����ѧ��:");
		//scanf("%s", send_ID);
		//printf("���뷢��������:");
		//scanf("%s", send_name);
		//printf("���������ѧ��:");
		//scanf("%s", recieve_ID);
		//printf("�������������:");
		//scanf("%s", recieve_name);
		if (op == 1)
		{
			printf("ѹ��\n");
			int res = compress(file_name);
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
		}
	}
	return 0;
}
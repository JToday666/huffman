#include <stdio.h>
#include <stdlib.h>
int main() {
	char op = 0;
	char file_name[20];
	char send_ID[20];
	char send_name[20];
	char recieve_ID[20];
	char recieve_name[20];
	printf("1:压缩\t\t2:解压缩\n");
	printf("输入操作:");
	scanf("%d", op);
	if (op != 1 && op != 2) return -1;
	else {
		printf("输入文件名:");
		scanf("%s", &file_name);
		printf("输入发送人学号:");
		scanf("%s", &send_ID);
		printf("输入发送人姓名:");
		scanf("%s", &send_name);
		printf("输入接收人学号:");
		scanf("%s", &recieve_ID);
		printf("输入接收人姓名:");
		scanf("%s", &recieve_name);
		if (op == 1)
		{
			char* massage = (char*)malloc();

		}
		else
		{

		}
	}
}
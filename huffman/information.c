#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <Windows.h>

//��ȡ������Ϣ
void info(char* message)
{
	char send_ID[20];
	char send_name[20];
	char recieve_ID[20];
	char recieve_name[20];

	printf("���뷢����ѧ��:");
	scanf("%s", send_ID);
	printf("���뷢��������:");
	scanf("%s", send_name);
	printf("���������ѧ��:");
	scanf("%s", recieve_ID);
	printf("�������������:");
	scanf("%s", recieve_name);

	strcpy(message, send_ID);
	strcat(message, ",");
	strcat(message, send_name);
	strcat(message, "\n");
	strcat(message, recieve_ID);
	strcat(message, ",");
	strcat(message, recieve_name);
	strcat(message, "\n");
}

static UINT g_OriginalInputCP;
static UINT g_OriginalOutputCP;
//��ȡ������Ϣ  
void inquire_base(char* sender, char* reciever)
{
	g_OriginalInputCP = GetConsoleCP();
	g_OriginalOutputCP = GetConsoleOutputCP();
	SetConsoleCP(65001);       // ���������ΪUTF-8
	//SetConsoleOutputCP(65001); // ���������ΪUTF-8������������룩

	char send_ID[20];
	char send_name[20];
	char recieve_ID[20];
	char recieve_name[20];

	printf("���뷢����ѧ��:");
	scanf("%s", send_ID);
	printf("���뷢��������:");
	scanf("%s", send_name);
	printf("���������ѧ��:");
	scanf("%s", recieve_ID);
	printf("�������������:");
	scanf("%s", recieve_name);

	strcpy(sender, send_ID);
	strcat(sender, ",");
	strcat(sender, send_name);
	//strcat(message, "\n");
	strcpy(reciever, recieve_ID);
	strcat(reciever, ",");
	strcat(reciever, recieve_name);
	//strcat(message, "\n");

	SetConsoleCP(g_OriginalInputCP);
	SetConsoleOutputCP(g_OriginalOutputCP);
}
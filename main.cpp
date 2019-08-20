
#include "header.h"

struct sockaddr_in remote_server;	//����� � ����� ����� ���������� �������
struct sockaddr_in remote_client;	//����� ���������� �������
struct sockaddr_in local_server;	//�������� ����� � ����� ����� ���������� �������
SOCKET s_remote_server;				//����� ��� ������� � ��������� �������
SOCKET s_local_server;				//����� ��� ������� � ��������� ��������
int remote_client_len;
int remote_server_len;
char client_IP[256];			//����������� IP ����� �������

void main(void)
{
	WSADATA wsadata;			//�������� ���������� � ������ ������
								//Widows Sockets DLL
	FILE* pFile = NULL;			//��������� �� ����������� ����
	unsigned threadID;			//���� ������������ ���������� ������
	char remote_server_IP[256];	//IP ����� ���������� �������
	char local_server_IP[256];	//IP ����� ���������� �������
	char remote_server_port[256];	//���� ���������� �������
	char local_server_port[256];	//���� ���������� �������

	CreateWindow("BUTTON", "UDP_PortMapper", 0, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

	printf("Content-type: text/plain\r\n\r\n");
	printf("UDP_PortMapper 1.0\n");
	printf("Copyright (C) 2004 Konstantyn Zvonarev\n");
	printf("zvonarev@mail.ru\n\n\n");

	//�������������� ������������� Windows Sockets DLL
	if ( WSAStartup(MAKEWORD(2,2),&wsadata) != 0 )
	{
		printf("Error calling WSAStartup()...\n");
		printf("Program terminating...\n");
		return;
	}
	printf("WSA initialized...\n");
	
	remote_client_len = sizeof(remote_client);
	remote_server_len = sizeof(remote_server);

	//������ ��������� �� �����
	printf("Reading settings from UDP_PortMapper.ini...\n");
	pFile = fopen("UDP_PortMapper.ini", "r");
	if (pFile != NULL)
	{
		fgets(remote_server_IP, 256, pFile);
		strtok(remote_server_IP, " ");
		printf("\nRemote server IP: %s\n",remote_server_IP);
		fgets(remote_server_port, 256, pFile);
		strtok(remote_server_port, " ");
		printf("Remote server port: %s\n", remote_server_port);
		fgets(local_server_IP, 256, pFile);
		strtok(local_server_IP, " ");
		printf("Local server IP: %s\n", local_server_IP);
		fgets(local_server_port, 256, pFile);
		strtok(local_server_port, " ");
		printf("Local server port: %s\n", local_server_port);
		fgets(client_IP, 256, pFile);
		strtok(client_IP, " ");
		printf("Allowed client IP (only this client may connect to server): %s\n", client_IP);
		fclose(pFile);
	}
	else
	{
		printf("UDP_PortMapper.ini not found...\n");
		printf("Program terminating...\n");
		return;
	}

	//��������� ��������� remote_server
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons( atoi(remote_server_port) );
	remote_server.sin_addr.s_addr = inet_addr(remote_server_IP);

	//��������� ��������� local_server
	local_server.sin_family = AF_INET;
	local_server.sin_port = htons( atoi(local_server_port) );
	local_server.sin_addr.s_addr = inet_addr(local_server_IP);

	//������� ����� ��� �������������� ������� � ����� ��������
	s_local_server = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_local_server == SOCKET_ERROR)
	{
		printf("error calling socket()...\n");
		return;
	}
	//����������� ��������� ����� � ���������� ����������
	if ( bind(s_local_server, (struct sockaddr*)&local_server, sizeof(local_server)) == SOCKET_ERROR )
	{
		printf("error calling bind()...\n");
		closesocket(s_local_server);
		return;
	}
	
	//������� ����� ��� �������������� ������ ������� � ��������� ��������
	s_remote_server = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_remote_server == SOCKET_ERROR)
	{
		printf("error calling socket()...\n");
		return;
	}


	//��������� ����� ��� ������������ ���������� Client->Server
	_beginthreadex(NULL,					//security
					2097152,				//stack_size (2 ��)
					&ClientToServer,		//thread function 
					NULL,					//������� �-�� ������
					0,						//�������� ����� ����� �� 
					&threadID);
	//��������� ����� ��� ������������ ���������� Server->Client
/*	_beginthreadex(NULL,					//security
					2097152,				//stack_size (2 ��)
					&ServerToClient,		//thread function 
					NULL,					//������� �-�� ������
					0,						//�������� ����� ����� �� 
					&threadID);*/

	Sleep(INFINITE);
}
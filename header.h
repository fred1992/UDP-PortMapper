
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 10240 //������ ����������

//------------------------------------------------------------------------
//------------------------��������� �������-------------------------------
//------------------------------------------------------------------------
unsigned _stdcall ClientToServer(void *arg);
unsigned _stdcall ServerToClient(void *arg);



//-------------------------------------------------------------------------
//------------------------���������� ����������----------------------------
//-------------------------------------------------------------------------
extern struct sockaddr_in remote_server;	//����� � ����� ����� ���������� �������
extern struct sockaddr_in remote_client;	//����� ���������� �������
extern struct sockaddr_in local_server;		//�������� ����� � ����� ����� ���������� �������
extern 	SOCKET s_remote_server;				//����� ��� �������������� � ��������� ��������
extern	SOCKET s_local_server;				//����� ��� �������������� � ��������� ��������
extern int remote_client_len;
extern int remote_server_len;
extern char client_IP[256];					//����������� IP ����� �������

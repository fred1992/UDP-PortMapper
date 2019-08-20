
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 10240 //размер датаграммы

//------------------------------------------------------------------------
//------------------------ПРОТОТИПЫ ФУНКЦИЙ-------------------------------
//------------------------------------------------------------------------
unsigned _stdcall ClientToServer(void *arg);
unsigned _stdcall ServerToClient(void *arg);



//-------------------------------------------------------------------------
//------------------------ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ----------------------------
//-------------------------------------------------------------------------
extern struct sockaddr_in remote_server;	//адрес и номер порта удаленного сервера
extern struct sockaddr_in remote_client;	//адрес удаленного клиента
extern struct sockaddr_in local_server;		//содержит адрес и номер порта локального сервера
extern 	SOCKET s_remote_server;				//сокет для взаимодействия с удаленным сервером
extern	SOCKET s_local_server;				//сокет для взаимодействия с удаленным клиентом
extern int remote_client_len;
extern int remote_server_len;
extern char client_IP[256];					//разрешенный IP адрес клиента

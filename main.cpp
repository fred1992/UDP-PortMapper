
#include "header.h"

struct sockaddr_in remote_server;	//адрес и номер порта удаленного сервера
struct sockaddr_in remote_client;	//адрес удаленного клиента
struct sockaddr_in local_server;	//содержит адрес и номер порта локального сервера
SOCKET s_remote_server;				//сокет для общения с удаленным серверо
SOCKET s_local_server;				//сокет для общения с удаленным клиентом
int remote_client_len;
int remote_server_len;
char client_IP[256];			//разрешенный IP адрес клиента

void main(void)
{
	WSADATA wsadata;			//содержит информацию о номере версии
								//Widows Sockets DLL
	FILE* pFile = NULL;			//указатель на открываемый файл
	unsigned threadID;			//сюда возвращается дескриптор потока
	char remote_server_IP[256];	//IP адрес удаленного сервера
	char local_server_IP[256];	//IP адрес локального сервера
	char remote_server_port[256];	//порт удаленного сервера
	char local_server_port[256];	//порт локального сервера

	CreateWindow("BUTTON", "UDP_PortMapper", 0, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

	printf("Content-type: text/plain\r\n\r\n");
	printf("UDP_PortMapper 1.0\n");
	printf("Copyright (C) 2004 Konstantyn Zvonarev\n");
	printf("zvonarev@mail.ru\n\n\n");

	//инициализируем использование Windows Sockets DLL
	if ( WSAStartup(MAKEWORD(2,2),&wsadata) != 0 )
	{
		printf("Error calling WSAStartup()...\n");
		printf("Program terminating...\n");
		return;
	}
	printf("WSA initialized...\n");
	
	remote_client_len = sizeof(remote_client);
	remote_server_len = sizeof(remote_server);

	//читаем настройки из файла
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

	//заполняем структуру remote_server
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons( atoi(remote_server_port) );
	remote_server.sin_addr.s_addr = inet_addr(remote_server_IP);

	//заполняем структуру local_server
	local_server.sin_family = AF_INET;
	local_server.sin_port = htons( atoi(local_server_port) );
	local_server.sin_addr.s_addr = inet_addr(local_server_IP);

	//создаем сокет для взаимодействия клиента с нашим сервером
	s_local_server = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_local_server == SOCKET_ERROR)
	{
		printf("error calling socket()...\n");
		return;
	}
	//привязываем созданный сокет к локальному интерфейсу
	if ( bind(s_local_server, (struct sockaddr*)&local_server, sizeof(local_server)) == SOCKET_ERROR )
	{
		printf("error calling bind()...\n");
		closesocket(s_local_server);
		return;
	}
	
	//создаем сокет для взаимодействия нашего сервера с удаленным сервером
	s_remote_server = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_remote_server == SOCKET_ERROR)
	{
		printf("error calling socket()...\n");
		return;
	}


	//запускаем поток для обслуживания соединения Client->Server
	_beginthreadex(NULL,					//security
					2097152,				//stack_size (2 Мб)
					&ClientToServer,		//thread function 
					NULL,					//аргумет ф-ии потока
					0,						//начинаем поток сразу же 
					&threadID);
	//запускаем поток для обслуживания соединения Server->Client
/*	_beginthreadex(NULL,					//security
					2097152,				//stack_size (2 Мб)
					&ServerToClient,		//thread function 
					NULL,					//аргумет ф-ии потока
					0,						//начинаем поток сразу же 
					&threadID);*/

	Sleep(INFINITE);
}
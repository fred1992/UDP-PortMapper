
#include "header.h"

unsigned _stdcall ClientToServer(void *arg)
{
	int rc;
	char buf[BUFSIZE];
	unsigned threadID;			//сюда возвращается дескриптор потока
	int i = 0;

	printf("Waiting client connection...\n");
	//бесконечный цикл приема UDP датаграм от клиента и отсылка их к серверу
	for (;;)
	{
		ZeroMemory(buf, sizeof(buf));
		
		//получаем датаграмму от удаленного клиента
		rc = recvfrom(s_local_server, buf, sizeof(buf), 0, 
						(struct sockaddr *)&remote_client, &remote_client_len);
		
		//проверяем IP адрес клиента
		if ( remote_client.sin_addr.s_addr != inet_addr(client_IP) )
		{
			printf("Client IP %s not allowed. Access denyed!\n", inet_ntoa(remote_client.sin_addr));
			continue;
		}

		//ничего не прочитанно или ошибка
		if (rc <= 0)
			continue;
		printf("Receved from client %i bytes\n", rc);

		//посылаем данные удаленному серверу
		rc = sendto(s_remote_server, buf, rc, 0,
						(struct sockaddr *)&remote_server, remote_server_len);
		if (rc <= 0)
			continue;
		printf("Sended to remote server %i bytes\n", rc);

		if (i == 0)
		{
			//запускаем поток для обслуживания соединения Server->Client
			_beginthreadex(NULL,				//security
						2097152,				//stack_size (2 Мб)
						&ServerToClient,		//thread function 
						NULL,					//аргумет ф-ии потока
						0,						//начинаем поток сразу же 
						&threadID);
			i = 1;
		}
	}
	return 0;
}
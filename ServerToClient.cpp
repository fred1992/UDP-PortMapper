
#include "header.h"

unsigned _stdcall ServerToClient(void *arg)
{
	char buf[BUFSIZE];
	int rc; 

	//бесконечный цикл приема UDP датаграм от сервера и отсылка их клиенту
	for (;;)
	{
		ZeroMemory(buf, sizeof(buf));
		
		//получаем датаграмму от удаленного сервера
		rc = recvfrom(s_remote_server, buf, sizeof(buf), 0, 
						(struct sockaddr *)&remote_server, &remote_server_len);
		//ничего не прочитанно или ошибка
		if (rc <= 0)
			continue;
		printf("Reseved from remote server %i bytes\n", rc);
		

		//посылаем данные удаленному клиенту
		rc = sendto(s_local_server, buf, rc, 0,
						(struct sockaddr *)&remote_client, remote_client_len);
		if (rc <= 0)
			continue;
		printf("Sended to remote client %i bytes\n", rc);
	}
	return 0;
}
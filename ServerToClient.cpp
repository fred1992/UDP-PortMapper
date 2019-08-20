
#include "header.h"

unsigned _stdcall ServerToClient(void *arg)
{
	char buf[BUFSIZE];
	int rc; 

	//����������� ���� ������ UDP �������� �� ������� � ������� �� �������
	for (;;)
	{
		ZeroMemory(buf, sizeof(buf));
		
		//�������� ���������� �� ���������� �������
		rc = recvfrom(s_remote_server, buf, sizeof(buf), 0, 
						(struct sockaddr *)&remote_server, &remote_server_len);
		//������ �� ���������� ��� ������
		if (rc <= 0)
			continue;
		printf("Reseved from remote server %i bytes\n", rc);
		

		//�������� ������ ���������� �������
		rc = sendto(s_local_server, buf, rc, 0,
						(struct sockaddr *)&remote_client, remote_client_len);
		if (rc <= 0)
			continue;
		printf("Sended to remote client %i bytes\n", rc);
	}
	return 0;
}
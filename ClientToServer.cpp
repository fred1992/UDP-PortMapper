
#include "header.h"

unsigned _stdcall ClientToServer(void *arg)
{
	int rc;
	char buf[BUFSIZE];
	unsigned threadID;			//���� ������������ ���������� ������
	int i = 0;

	printf("Waiting client connection...\n");
	//����������� ���� ������ UDP �������� �� ������� � ������� �� � �������
	for (;;)
	{
		ZeroMemory(buf, sizeof(buf));
		
		//�������� ���������� �� ���������� �������
		rc = recvfrom(s_local_server, buf, sizeof(buf), 0, 
						(struct sockaddr *)&remote_client, &remote_client_len);
		
		//��������� IP ����� �������
		if ( remote_client.sin_addr.s_addr != inet_addr(client_IP) )
		{
			printf("Client IP %s not allowed. Access denyed!\n", inet_ntoa(remote_client.sin_addr));
			continue;
		}

		//������ �� ���������� ��� ������
		if (rc <= 0)
			continue;
		printf("Receved from client %i bytes\n", rc);

		//�������� ������ ���������� �������
		rc = sendto(s_remote_server, buf, rc, 0,
						(struct sockaddr *)&remote_server, remote_server_len);
		if (rc <= 0)
			continue;
		printf("Sended to remote server %i bytes\n", rc);

		if (i == 0)
		{
			//��������� ����� ��� ������������ ���������� Server->Client
			_beginthreadex(NULL,				//security
						2097152,				//stack_size (2 ��)
						&ServerToClient,		//thread function 
						NULL,					//������� �-�� ������
						0,						//�������� ����� ����� �� 
						&threadID);
			i = 1;
		}
	}
	return 0;
}
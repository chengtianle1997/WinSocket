
#include<iostream>

#include<string>

#include"ClientNet.h"



using namespace std;



int main()

{

	ClientNet client;

	int rlt = 0;

	string msg;

	//memset(msg, 0, sizeof(msg));



	//���ӵ�127.0.0.1�������أ�,�˿ں�Ϊ8888�ķ����

	printf("connecting.....\n");

	rlt = client.ClientConnect(8888, "127.0.0.1");



	if (rlt == 0)

		// socket���ӳɹ�

	{

		//������Ϣ

		printf("	connect successfully. input  q to quit\n");

		printf("-------------------------\n");

		while (1)

		{

			printf("msg input: ");

			getline(cin, msg);

			if (msg == "q")

				break;

			else

			{

				printf("sending msg.....\n");

				rlt = client.ClientSend(msg.c_str(), msg.length());

			}



		}





		// �ر�socket

		printf("closing socket.....\n");

		client.ClientClose();





	}



	system("pause");



	return 0;



}

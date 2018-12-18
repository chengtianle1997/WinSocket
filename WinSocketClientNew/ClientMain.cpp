
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



	//连接到127.0.0.1（即本地）,端口号为8888的服务端

	printf("connecting.....\n");

	rlt = client.ClientConnect(8888, "127.0.0.1");



	if (rlt == 0)

		// socket连接成功

	{

		//发送消息

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





		// 关闭socket

		printf("closing socket.....\n");

		client.ClientClose();





	}



	system("pause");



	return 0;



}

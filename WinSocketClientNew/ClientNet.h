
#pragma once



/*

ClinetNet.h

�ͻ�����

*/



#include<stdio.h>

#include<windows.h>



#pragma comment(lib, "Ws2_32.lib")



class ClientNet

{

private:

	SOCKET m_sock;



public:

	// ����ָ���ķ�����

	int ClientConnect(int port, const char* address);

	// ������Ϣ

	int ClientSend(const char* msg, int len);

	// �ر�����

	void ClientClose();

};


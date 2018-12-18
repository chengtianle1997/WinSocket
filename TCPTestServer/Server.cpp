
// JTCPserver.cpp : ��������װ��TCP�����



//#include "stdafx.h"

#include <iostream>

#include <string>

#include <WinSock2.h> // WinSocket 

#include <WS2tcpip.h> // IP��ַת���õ�inet_pton

#pragma comment(lib,"ws2_32.lib")  

using namespace std;



// ��1����ʼ��WinSock

bool initWinSock();

// ��2������socket

bool createSocket(SOCKET &listenScok);

// ��3��socket�󶨱�����ַ��Ϣ

bool bindIPandPort(SOCKET &listenScok, const string ip, const unsigned short port);

// ��4������socket�����տͻ�������

bool listenSocket(SOCKET &listenScok);

// ��5���ȴ��ͻ�������-����

bool waitClientConnect(SOCKET &listenScok, SOCKET &clientSock);

// ��6����������-����

bool receiveData(SOCKET &clientSock, string &data);

// ��7��ֹͣ�׽��ֵĽ��ա�����

bool shutdownSocket(SOCKET &clientSock);

// ��8��������Ϣ

bool sendData(SOCKET &clientSock, const string &data);



int JTCPserver()

{

	SOCKET listenScok;	// �����Socket

	SOCKET clientSock;	// �ͻ���Socket

	string data;		// �յ�������



	// ��1����ʼ��WinSock

	initWinSock();

	// ��2������socket

	createSocket(listenScok);

	// ��3��socket�󶨱�����ַ��Ϣ

	bindIPandPort(listenScok, "127.0.0.1", 1994);

	// ��4������socket�����տͻ�������

	listenSocket(listenScok);



	// ���ȿͻ�������

	bool isClientSockConnect = false;	// �Ƿ��пͻ���������

	bool isReceiveData = false;			// �Ƿ�������ݳɹ�



	while (true)

	{

		if (!isClientSockConnect)

		{

			// ��5���ȴ��ͻ�������

			isClientSockConnect = waitClientConnect(listenScok, clientSock);

		}

		else

		{

			if (!isReceiveData)

			{

				// ��6����������-����

				isReceiveData = receiveData(clientSock, data);

				// �����������ʧ����Ͽ�

				if (!isReceiveData)

				{

					// ��7��ֹͣ�׽��ֵĽ��ա�����

					shutdownSocket(clientSock);

					cout << "�ȴ��ͻ���������..." << endl;

					isClientSockConnect = false; // ����������

				}

			}

			if (isReceiveData && data != "jfzpoi"  && data != "@end#")

			{

				isReceiveData = false;

			}

			if (isReceiveData && data == "jfzpoi")

			{

				// ��8��������Ϣ(�յ�����Ϊjfzpoi)

				sendData(clientSock, "sandeepin!\r\n");

				isReceiveData = false;

			}

			if (isReceiveData && data == "@end#")

			{

				// ��9���ر����

				int err;

				//				err = shutdown(listenScok, 2);

				//				if (err == SOCKET_ERROR)

				//				{

				//					cout << "�ر�ʧ�ܣ�" << endl;

				//				}

								// �ر��׽��֣��ͷ���Դ

				err = closesocket(listenScok);

				if (err == SOCKET_ERROR)

				{

					cout << "�ر�socketʧ�ܣ�" << endl;

				}

				// ֹͣʹ��WinSock�⣬�ͷŶ�Ӧ��Դ

				if (WSACleanup() != 0)

				{

					cout << "WSA���ʧ�ܣ�" << endl;

				}

				cout << "�����ˣ����ȹػ���poi" << endl;

				return 0;

			}

		}

	}

}



// ��1����ʼ��WinSock

bool initWinSock()

{

	WORD verision = MAKEWORD(2, 2);

	WSADATA lpData;

	int intEr = WSAStartup(verision, &lpData); // ָ��winsock�汾����ʼ��

	if (intEr != 0)

	{

		cout << "WinSock��ʼ��ʧ�ܣ�" << endl;

		return false;

	}

	cout << "WinSock��ʼ���ɹ���" << endl;

	return true;

}



// ��2������socket

bool createSocket(SOCKET &listenScok)

{

	// ��������socket  

	listenScok = socket(AF_INET, SOCK_STREAM, 0);

	if (listenScok == INVALID_SOCKET)

	{

		cout << "socket����ʧ�ܣ�" << endl;

		return false;

	}

	cout << "socket�����ɹ���" << endl;

	return true;

}



// ��3��socket�󶨱�����ַ��Ϣ

bool bindIPandPort(SOCKET &listenScok, const string ip, const unsigned short port)

{

	// ����sockaddr_in�ṹ��

	// ��bind������connect�������ᵽ���׽��ֱ�������ַ��Ϣ�ṹ��const struct sockaddr��const struct sockaddr_i

	sockaddr_in hostAddr;

	hostAddr.sin_family = AF_INET;

	hostAddr.sin_port = htons(port);//ת���������ֽ���  

									//hostAddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);//ת���������ֽ���  

									//cout << "net IP:" << hostAddr.sin_addr.S_un.S_addr << endl;  

									/*

									inet_addr()�汾̫�ͣ�������ʹ��inet_pton(Э���壬�ַ���IP��ַ��voidĿ��in_addr*)

									ͷ�ļ���WS2tcpip.h

									*/

	in_addr addr;

	inet_pton(AF_INET, ip.c_str(), (void*)&addr);

	hostAddr.sin_addr = addr;

	cout << "ip(�����ֽ���):" << addr.S_un.S_addr << endl;

	cout << "ip(������ʽ):" << ip.c_str() << endl;



	// �����׽���listenSock�󶨱�����ַ��Ϣ  

	int err = bind(listenScok, (struct sockaddr*)&hostAddr, sizeof(sockaddr));

	if (err != 0)

	{

		cout << "����IP��ʧ�ܣ�" << endl;

		return false;

	}

	return true;

}



// ��4������socket�����տͻ�������

bool listenSocket(SOCKET &listenScok)

{

	// �趨�׽���Ϊ����״̬��׼�����տͻ������̷���������������

	int err = listen(listenScok, 3);

	if (err != 0)

	{

		cout << "socket����ʧ�ܣ�" << endl;

		return false;

	}

	cout << "�����ͻ��������С���" << endl;

	return true;

}



// ��5���ȴ��ͻ�������-����

bool waitClientConnect(SOCKET &listenScok, SOCKET &clientSock)

{

	sockaddr_in clientAddr;

	int len = sizeof(struct sockaddr); // ����ָ�����ȣ�����ᵼ��accept����10014����

									   // accept��ѭ���ȴ��ͻ�������

	clientSock = accept(listenScok, (struct sockaddr*)&clientAddr, &len);

	cout << "�ͻ���Socket���:" << clientSock << endl;

	if (clientSock == INVALID_SOCKET)

	{

		cout << "�ͻ�������ʧ�ܣ�" << endl;

		cout << WSAGetLastError() << endl;

		return false;

	}

	return true;

}



// ��6����������-����

bool receiveData(SOCKET &clientSock, string &data)

{

	static int cnt = 1; // �������ݱ��-��̬

						// ͨ���ѽ������ӵ��׽��֣��������� �趨����1024�ֽ�

	char buf[1024] = "\0";

	// flags������ʽ��0�������ݣ�MSG_PEEDϵͳ�����������ݸ��Ƶ����ṩ�Ľ��ջ������ڣ�ϵͳ����������δɾ����MSG_OOB����������ݣ�ͨ���ò���0���ɣ�

	int buflen = recv(clientSock, buf, 1024, 0);

	if (buflen == SOCKET_ERROR)

	{

		cout << "����ʧ�ܣ�" << endl;

		return false;

	}

	// һ����������ʾ��������

	data = string(buf);

	cout << "�յ���" << cnt++ << "�Σ�����Ϊ:\n" << buf << endl;

	return true;

}



// ��7��ֹͣ�׽��ֵĽ��ա�����

bool shutdownSocket(SOCKET &clientSock)

{

	//������͹أ�ֹͣ�׽��ֵĽ��ա����͹��ܣ�0��ֹ���գ�1��ֹ���ͣ�2���ƽ��շ��ͣ�

	int err = shutdown(clientSock, 2);

	if (err == SOCKET_ERROR)

	{

		cout << "�ر�Socketʧ�ܣ�" << endl;

		return false;

	}

	return true;

}



// ��8��������Ϣ

bool sendData(SOCKET &clientSock, const string &data)

{



	int err = send(clientSock, data.c_str(), data.size(), 0);

	if (err == SOCKET_ERROR)

	{

		cout << "����ʧ�ܣ�" << endl;

		return false;

	}

	cout << "��������Ϊ:\n" << data << endl;

	return true;

}

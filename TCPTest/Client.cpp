
// JTCPclient.cpp : ��������װ��TCP�ͻ��˴���



//#include "stdafx.h"  

#include <iostream>

#include <WinSock2.h>  

#include <WS2tcpip.h>  

#pragma comment(lib,"ws2_32.lib")

using namespace std;



// ��1����ʼ��WinSock

bool initWinSockC();

// ��2������socket

bool createSocketC(SOCKET &listenScok);

// ��3�����ӵ�������

bool connectSocketC(SOCKET &conSock, const string ip, const unsigned short port);

// ��4����������

bool sendDataC(SOCKET &clientSock, const string &data);

// ��5����������

bool receiveDataC(SOCKET &clientSock, string &data);



int JTCPclient()

{

	SOCKET clientSock;	// �ͻ���Socket

	string data;		// �յ�������



	bool isCreateSocket = false;	// �Ƿ񴴽���Socket

	bool isConnectSocket = false;	// �Ƿ������˷�����

	bool isSendDataOK = false;		// �Ƿ��ͳɹ�����

	bool isReceiveDataOK = false;	// �Ƿ���ճɹ�����



	// ��1����ʼ��WinSock

	if (initWinSockC())

	{

		while (true)

		{

			if (!isCreateSocket)

			{

				// ��2������socket

				createSocketC(clientSock);

				isCreateSocket = true;

			}

			else

			{

				if (!isConnectSocket)

				{

					// ��3�����ӵ�������

					connectSocketC(clientSock, "127.0.0.1", 1994);

					isConnectSocket = true;

				}

				else

				{

					if (!isSendDataOK)

					{

						// ��4����������

						isSendDataOK = sendDataC(clientSock, "jfz hello\r\n");

					}

					else

					{

						if (!isReceiveDataOK)

						{

							// ��5����������

							isReceiveDataOK = receiveDataC(clientSock, data);

						}

						else

						{

							if (data == "@end#")

							{

								WSACleanup();

								return 0;

							}

							isReceiveDataOK = false;

							isSendDataOK = false;

						}

					}



				}

			}

		}

	}

	return 0;

}



// ��1����ʼ��WinSock

bool initWinSockC()

{

	WORD verision = MAKEWORD(2, 2);

	WSADATA lpData;

	int intEr = WSAStartup(verision, &lpData); // ָ��winsock�汾����ʼ��

	if (intEr != 0)

	{

		std::cout << "WinSock��ʼ��ʧ�ܣ�" << endl;

		return false;

	}

	std::cout << "WinSock��ʼ���ɹ���" << endl;

	return true;

}



// ��2������socket

bool createSocketC(SOCKET &listenScok)

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



// ��3�����ӵ�������

bool connectSocketC(SOCKET &conSock, const string ip, const unsigned short port)

{

	// ������ַ�ṹ��

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



	// ������������������

	int err = connect(conSock, (sockaddr*)&hostAddr, sizeof(sockaddr));

	if (err == INVALID_SOCKET)

	{

		cout << "���ӷ�����ʧ�ܣ�" << endl;

		return false;

	}

	return true;

}



// ��4����������

bool sendDataC(SOCKET &clientSock, const string &data)

{

	int err = send(clientSock, data.c_str(), data.size(), 0);

	if (err == SOCKET_ERROR)

	{

		cout << "����ʧ�ܣ�" << endl;

		return false;

	}

	cout << "��������Ϊ:\n" << data.c_str() << endl;

	return true;

}



// ��5����������

bool receiveDataC(SOCKET &clientSock, string &data)

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

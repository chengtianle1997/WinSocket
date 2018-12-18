
// JTCPclient.cpp : 蒋方正封装的TCP客户端代码



//#include "stdafx.h"  

#include <iostream>

#include <WinSock2.h>  

#include <WS2tcpip.h>  

#pragma comment(lib,"ws2_32.lib")

using namespace std;



// 【1】初始化WinSock

bool initWinSockC();

// 【2】创建socket

bool createSocketC(SOCKET &listenScok);

// 【3】连接到服务器

bool connectSocketC(SOCKET &conSock, const string ip, const unsigned short port);

// 【4】发送数据

bool sendDataC(SOCKET &clientSock, const string &data);

// 【5】接收数据

bool receiveDataC(SOCKET &clientSock, string &data);



int JTCPclient()

{

	SOCKET clientSock;	// 客户端Socket

	string data;		// 收到的数据



	bool isCreateSocket = false;	// 是否创建了Socket

	bool isConnectSocket = false;	// 是否连上了服务器

	bool isSendDataOK = false;		// 是否发送成功数据

	bool isReceiveDataOK = false;	// 是否接收成功数据



	// 【1】初始化WinSock

	if (initWinSockC())

	{

		while (true)

		{

			if (!isCreateSocket)

			{

				// 【2】创建socket

				createSocketC(clientSock);

				isCreateSocket = true;

			}

			else

			{

				if (!isConnectSocket)

				{

					// 【3】连接到服务器

					connectSocketC(clientSock, "127.0.0.1", 1994);

					isConnectSocket = true;

				}

				else

				{

					if (!isSendDataOK)

					{

						// 【4】发送数据

						isSendDataOK = sendDataC(clientSock, "jfz hello\r\n");

					}

					else

					{

						if (!isReceiveDataOK)

						{

							// 【5】接收数据

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



// 【1】初始化WinSock

bool initWinSockC()

{

	WORD verision = MAKEWORD(2, 2);

	WSADATA lpData;

	int intEr = WSAStartup(verision, &lpData); // 指定winsock版本并初始化

	if (intEr != 0)

	{

		std::cout << "WinSock初始化失败！" << endl;

		return false;

	}

	std::cout << "WinSock初始化成功！" << endl;

	return true;

}



// 【2】创建socket

bool createSocketC(SOCKET &listenScok)

{

	// 创建侦听socket  

	listenScok = socket(AF_INET, SOCK_STREAM, 0);

	if (listenScok == INVALID_SOCKET)

	{

		cout << "socket创建失败！" << endl;

		return false;

	}

	cout << "socket创建成功！" << endl;

	return true;

}



// 【3】连接到服务器

bool connectSocketC(SOCKET &conSock, const string ip, const unsigned short port)

{

	// 建立地址结构体

	sockaddr_in hostAddr;

	hostAddr.sin_family = AF_INET;

	hostAddr.sin_port = htons(port);//转换成网络字节序  

									//hostAddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);//转换成网络字节序  

									//cout << "net IP:" << hostAddr.sin_addr.S_un.S_addr << endl;  

									/*

									inet_addr()版本太低，被弃用使用inet_pton(协议族，字符串IP地址，void目标in_addr*)

									头文件：WS2tcpip.h

									*/

	in_addr addr;

	inet_pton(AF_INET, ip.c_str(), (void*)&addr);

	hostAddr.sin_addr = addr;

	cout << "ip(网络字节序):" << addr.S_un.S_addr << endl;

	cout << "ip(常规形式):" << ip.c_str() << endl;



	// 向服务器提出连接请求

	int err = connect(conSock, (sockaddr*)&hostAddr, sizeof(sockaddr));

	if (err == INVALID_SOCKET)

	{

		cout << "连接服务器失败！" << endl;

		return false;

	}

	return true;

}



// 【4】发送数据

bool sendDataC(SOCKET &clientSock, const string &data)

{

	int err = send(clientSock, data.c_str(), data.size(), 0);

	if (err == SOCKET_ERROR)

	{

		cout << "发送失败！" << endl;

		return false;

	}

	cout << "发送数据为:\n" << data.c_str() << endl;

	return true;

}



// 【5】接收数据

bool receiveDataC(SOCKET &clientSock, string &data)

{

	static int cnt = 1; // 接收数据编号-静态

						// 通过已建立连接的套接字，接收数据 设定缓冲1024字节

	char buf[1024] = "\0";

	// flags操作方式（0正常数据，MSG_PEED系统缓冲区的数据复制到所提供的接收缓冲区内，系统缓冲区数据未删除，MSG_OOB处理带外数据，通常用参数0即可）

	int buflen = recv(clientSock, buf, 1024, 0);

	if (buflen == SOCKET_ERROR)

	{

		cout << "接收失败！" << endl;

		return false;

	}

	// 一切正常则显示接收数据

	data = string(buf);

	cout << "收到第" << cnt++ << "次，内容为:\n" << buf << endl;

	return true;

}

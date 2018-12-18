
// JTCPserver.cpp : 蒋方正封装的TCP服务端



//#include "stdafx.h"

#include <iostream>

#include <string>

#include <WinSock2.h> // WinSocket 

#include <WS2tcpip.h> // IP地址转换用到inet_pton

#pragma comment(lib,"ws2_32.lib")  

using namespace std;



// 【1】初始化WinSock

bool initWinSock();

// 【2】创建socket

bool createSocket(SOCKET &listenScok);

// 【3】socket绑定本机地址信息

bool bindIPandPort(SOCKET &listenScok, const string ip, const unsigned short port);

// 【4】侦听socket，接收客户端请求

bool listenSocket(SOCKET &listenScok);

// 【5】等待客户端连接-阻塞

bool waitClientConnect(SOCKET &listenScok, SOCKET &clientSock);

// 【6】接收数据-阻塞

bool receiveData(SOCKET &clientSock, string &data);

// 【7】停止套接字的接收、发送

bool shutdownSocket(SOCKET &clientSock);

// 【8】发送信息

bool sendData(SOCKET &clientSock, const string &data);



int JTCPserver()

{

	SOCKET listenScok;	// 服务端Socket

	SOCKET clientSock;	// 客户端Socket

	string data;		// 收到的数据



	// 【1】初始化WinSock

	initWinSock();

	// 【2】创建socket

	createSocket(listenScok);

	// 【3】socket绑定本机地址信息

	bindIPandPort(listenScok, "127.0.0.1", 1994);

	// 【4】侦听socket，接收客户端请求

	listenSocket(listenScok);



	// 坐等客户端连接

	bool isClientSockConnect = false;	// 是否有客户端连进来

	bool isReceiveData = false;			// 是否接收数据成功



	while (true)

	{

		if (!isClientSockConnect)

		{

			// 【5】等待客户端连接

			isClientSockConnect = waitClientConnect(listenScok, clientSock);

		}

		else

		{

			if (!isReceiveData)

			{

				// 【6】接收数据-阻塞

				isReceiveData = receiveData(clientSock, data);

				// 如果接收数据失败则断开

				if (!isReceiveData)

				{

					// 【7】停止套接字的接收、发送

					shutdownSocket(clientSock);

					cout << "等待客户端再连接..." << endl;

					isClientSockConnect = false; // 可以重连了

				}

			}

			if (isReceiveData && data != "jfzpoi"  && data != "@end#")

			{

				isReceiveData = false;

			}

			if (isReceiveData && data == "jfzpoi")

			{

				// 【8】发送信息(收的数据为jfzpoi)

				sendData(clientSock, "sandeepin!\r\n");

				isReceiveData = false;

			}

			if (isReceiveData && data == "@end#")

			{

				// 【9】关闭相关

				int err;

				//				err = shutdown(listenScok, 2);

				//				if (err == SOCKET_ERROR)

				//				{

				//					cout << "关闭失败！" << endl;

				//				}

								// 关闭套接字，释放资源

				err = closesocket(listenScok);

				if (err == SOCKET_ERROR)

				{

					cout << "关闭socket失败！" << endl;

				}

				// 停止使用WinSock库，释放对应资源

				if (WSACleanup() != 0)

				{

					cout << "WSA清空失败！" << endl;

				}

				cout << "关完了，坐等关机！poi" << endl;

				return 0;

			}

		}

	}

}



// 【1】初始化WinSock

bool initWinSock()

{

	WORD verision = MAKEWORD(2, 2);

	WSADATA lpData;

	int intEr = WSAStartup(verision, &lpData); // 指定winsock版本并初始化

	if (intEr != 0)

	{

		cout << "WinSock初始化失败！" << endl;

		return false;

	}

	cout << "WinSock初始化成功！" << endl;

	return true;

}



// 【2】创建socket

bool createSocket(SOCKET &listenScok)

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



// 【3】socket绑定本机地址信息

bool bindIPandPort(SOCKET &listenScok, const string ip, const unsigned short port)

{

	// 制作sockaddr_in结构体

	// 在bind函数，connect函数里提到了套接字编程网络地址信息结构体const struct sockaddr和const struct sockaddr_i

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



	// 侦听套接字listenSock绑定本机地址信息  

	int err = bind(listenScok, (struct sockaddr*)&hostAddr, sizeof(sockaddr));

	if (err != 0)

	{

		cout << "本地IP绑定失败！" << endl;

		return false;

	}

	return true;

}



// 【4】侦听socket，接收客户端请求

bool listenSocket(SOCKET &listenScok)

{

	// 设定套接字为侦听状态，准备接收客户机进程发送来的连接请求

	int err = listen(listenScok, 3);

	if (err != 0)

	{

		cout << "socket监听失败！" << endl;

		return false;

	}

	cout << "监听客户端连接中……" << endl;

	return true;

}



// 【5】等待客户端连接-阻塞

bool waitClientConnect(SOCKET &listenScok, SOCKET &clientSock)

{

	sockaddr_in clientAddr;

	int len = sizeof(struct sockaddr); // 必须指定长度，否则会导致accept返回10014错误

									   // accept会循环等待客户端连接

	clientSock = accept(listenScok, (struct sockaddr*)&clientAddr, &len);

	cout << "客户端Socket编号:" << clientSock << endl;

	if (clientSock == INVALID_SOCKET)

	{

		cout << "客户端连接失败！" << endl;

		cout << WSAGetLastError() << endl;

		return false;

	}

	return true;

}



// 【6】接收数据-阻塞

bool receiveData(SOCKET &clientSock, string &data)

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



// 【7】停止套接字的接收、发送

bool shutdownSocket(SOCKET &clientSock)

{

	//（收完就关）停止套接字的接收、发送功能（0禁止接收，1禁止发送，2禁制接收发送）

	int err = shutdown(clientSock, 2);

	if (err == SOCKET_ERROR)

	{

		cout << "关闭Socket失败！" << endl;

		return false;

	}

	return true;

}



// 【8】发送信息

bool sendData(SOCKET &clientSock, const string &data)

{



	int err = send(clientSock, data.c_str(), data.size(), 0);

	if (err == SOCKET_ERROR)

	{

		cout << "发送失败！" << endl;

		return false;

	}

	cout << "发送数据为:\n" << data << endl;

	return true;

}

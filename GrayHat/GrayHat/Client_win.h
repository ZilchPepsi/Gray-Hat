#pragma once

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>

#include "NetworkConstants.h"

#pragma comment(lib, "Ws2_32.lib")




class Client
{
public:
	Client(const char* ip);
	~Client();

	void start();
	int sendBytes(const char* const bytes, int size) const;
	int getStatus();

private:
	char recvbuf[RECVBUF_SIZE];
	int recvbuflen = RECVBUF_SIZE;
	const char* ipaddr;

	struct timeval recv_timeout, send_timeout;
	volatile int status;
	SOCKET c_socket;
	std::mutex socketMutex;
	WSADATA wsaData;
	std::thread thread;

	void _startClient();
};


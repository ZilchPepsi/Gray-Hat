#pragma once

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define PORT "10004"


class Server
{
public:
	Server();
	~Server();

	void start();
	int sendBytes(byte* bytes, int size);
	int getStatus();

private:
	int status;
	SOCKET socket;
	WSADATA wsaData;
	std::thread thread;

	void _startServer();
};


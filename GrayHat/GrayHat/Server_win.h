#pragma once

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>

#include "NetworkConstants.h"

#pragma comment(lib, "Ws2_32.lib")

struct Client_s {
	SOCKET* socket;
	std::mutex* lock;
	Client_s(SOCKET* s, std::mutex* l) :socket(s),lock(l) {}
	~Client_s() {
		delete lock;
		closesocket(*socket);
	}
};

class Server
{
public:
	Server();
	~Server();

	void start();
	int sendBytes(struct Client_s* client, const char* const bytes, int size) const;
	int sendBytes(SOCKET* client, const char* const bytes, int size) const;
	int getStatus();

private:
	struct Client_s *clients[MAX_CLIENTS];
	int clientsSize = 0;
	std::mutex clientMutex;

	char recvbuf[RECVBUF_SIZE];
	int recvbuflen = RECVBUF_SIZE;
	std::mutex recvMutex;

	struct timeval recv_timeout, send_timeout;
	volatile int status;
	SOCKET s_socket;
	WSADATA wsaData;
	std::thread thread;
	std::thread listenThread;

	void _startServer();
	void _listen();
	int removeClient(int);
};


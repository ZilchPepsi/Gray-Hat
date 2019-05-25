#pragma once

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

#define PORT "10004"
#define MAX_CLIENTS 3
#define RECV_TIMEOUT 1
#define SEND_TIMEOUT 3
#define SLEEP_TIME 17

#define RECVBUF_SIZE 1024

#define OK 0
#define DOWN 1
#define ERR 2

#define SERVER_FULL 3

struct Client {
	SOCKET* socket;
	std::mutex* lock;
	Client(SOCKET* s, std::mutex* l) :socket(s),lock(l) {}
	~Client() {
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
	int sendBytes(const SOCKET* client, const byte* const bytes, int size) const;
	int getStatus();

private:
	struct Client *clients[MAX_CLIENTS];
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
};


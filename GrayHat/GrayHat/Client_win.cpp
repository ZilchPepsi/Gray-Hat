#include "Client_win.h"



Client::Client(const char* ip):ipaddr(ip)
{
	c_socket = INVALID_SOCKET;
	status = DOWN;
	recv_timeout.tv_sec = RECV_TIMEOUT;
	recv_timeout.tv_usec = 0;
	send_timeout.tv_sec = SEND_TIMEOUT;
	send_timeout.tv_usec = 0;
}

Client::~Client()
{
	closesocket(c_socket);
	WSACleanup();

	if (thread.joinable()) {
		thread.join();
	}
}

int Client::getStatus() {
	return status;
}

void Client::start() {
	thread = std::thread(&Client::_startClient, this);
}

void Client::_startClient() {
	printf("client initializing\n");
		int iResult;

		if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
			printf("WSAStartup failed in client: %d\n", iResult);
			status = ERR;
			return;
		}

		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (iResult = getaddrinfo(ipaddr, PORT, &hints, &result)) {
			printf("getaddrinfo failed in client: %d\n", iResult);
			WSACleanup();
			status = ERR;
			return;
		}

		c_socket = INVALID_SOCKET;

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
				printf("WSAStartup failed in client: %d\n", iResult);
				status = ERR;
				return;
			}
			c_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (c_socket != INVALID_SOCKET) {
				if (!(iResult = connect(c_socket, ptr->ai_addr, (int)ptr->ai_addrlen))) {
					break;
				}
				else {
					closesocket(c_socket);
					c_socket = INVALID_SOCKET;
				}
			}

		}
		freeaddrinfo(result);

		if (c_socket == INVALID_SOCKET) {
			printf("client failed to connect\n");
			WSACleanup();
			status = ERR;
			return;
		}


		if (setsockopt(c_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_timeout, sizeof(recv_timeout)) < 0) {
			printf("setsockopt rcv_timeout failed: %d\n", WSAGetLastError());
			closesocket(c_socket);
			WSACleanup();
			return;
		}
		if (setsockopt(c_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&send_timeout, sizeof(send_timeout)) < 0) {
			printf("setsockopt snd_timeout failed: %d\n", WSAGetLastError());
			closesocket(c_socket);
			WSACleanup();
			return;
		}

		status = OK;
		printf("client connected, starting up\n");

		while (status == OK) {
			socketMutex.lock();
			if ((iResult = recv(c_socket, recvbuf, recvbuflen, 0))>0) {
				if (!(iResult == -1 && WSAGetLastError() == 10060)) {
					printf("recieved bytes from server: %d\n", iResult);
					//TODO recieved something from server
				}
			}
			else if (!iResult) {
				printf("client: connection was closed by server\n");
				status = DOWN;
			}
			else if (WSAGetLastError() != 10060) {
				printf("client receive failed with error: %d\n", WSAGetLastError());
				status = ERR;
			}
			socketMutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
		}
		
}

int Client::sendBytes(const char* const bytes, int size) const {
	int iSendResult;
	printf("client sending %c\n", *bytes);
	if ((iSendResult = send(c_socket, bytes, size, 0)) == SOCKET_ERROR) {
		printf("server send failed with error: %d\n", WSAGetLastError());
		return WSAGetLastError();
	}
	printf("Bytes sent to server: %d\n", iSendResult);
	return 0;
}
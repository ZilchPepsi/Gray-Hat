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

		//Resolve server address and port
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
					printf("client: connected to server\n");
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
		status = OK;


		while (status == OK) {
			
			if (iResult = recv(c_socket, recvbuf, recvbuflen, 0)) {
				printf("Bytes received from server: %d\n", iResult);
			}
			else if (iResult == 0) {
				printf("client: connection was closed by server\n");
				status = DOWN;
				shutdown(c_socket, SD_SEND);
			}
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				status = ERR;
			}
		}

		////test
		//const char* sendbuf = "test message";
		//if ((iResult = send(c_socket, sendbuf, (int)strlen(sendbuf), 0)) == SOCKET_ERROR) {
		//	printf("client send failed with error: %d\n", WSAGetLastError());
		//	closesocket(c_socket);
		//	WSACleanup();
		//	return;
		//}

		//printf("Bytes sent to server: %ld\n", iResult);

		////shut down connection

		//if (iResult = shutdown(c_socket, SD_SEND)) {
		//	printf("client shutdown failed with error: %d\n", WSAGetLastError());
		//	closesocket(c_socket);
		//	WSACleanup();
		//	return;
		//}

		//char recvbuf[1024];
		//int recvbuflen = 1024;
		
}

int Client::sendBytes(const byte* const bytes, int size) const {
	return -1;
}
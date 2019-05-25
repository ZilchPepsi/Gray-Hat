#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define PORT "10004"

enum SocketStatus {
	DOWN,
	UP,
	ERR
};

class ServerSocket {
public:
	int err;
	enum SocketStatus socketStatus;

	ServerSocket() {
		s_socket = INVALID_SOCKET;
		err = 0;
		socketStatus = DOWN;
	}
	~ServerSocket() {
		closesocket(s_socket);
		WSACleanup();
		if (serverThread.joinable()) {
			serverThread.join();
		}
	}

	void start() {
		serverThread = std::thread(&ServerSocket::_startServer,this);
	}

private:
	SOCKET s_socket;
	WSADATA wsaData;
	std::thread serverThread;

	void _startServer() {

		int iResult;
		if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
			printf("WSAStartup failed in server: %d\n", iResult);
			err  = 1;
			socketStatus = ERR;
			return;
		}

		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;


		//Resolve the local address and port to be used by the server
		if (iResult = getaddrinfo(NULL, PORT, &hints, &result)) {
			printf("getaddrinfo failed in server: %d\n", iResult);
			WSACleanup();
			err = 1;
			socketStatus = ERR;
			return;
		}

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
				printf("WSAStartup failed in server: %d\n", iResult);
				err = 1;
				socketStatus = ERR;
				return;
			}
			s_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (WSAGetLastError()) {
				printf("WSA had an error: %d\n", WSAGetLastError());
			}

			if (s_socket != INVALID_SOCKET) {
				if (!(iResult = bind(s_socket, ptr->ai_addr, (int)ptr->ai_addrlen))) {
					break;
				}
				else {
					closesocket(s_socket);
					s_socket = INVALID_SOCKET;
				}
			}
		}
		freeaddrinfo(result);

		if (s_socket == INVALID_SOCKET) {
			printf("socket is INVALID_SOCKET in server\n");
			WSACleanup();
			err = 1;
			socketStatus = ERR;
			return;
		}
		socketStatus = UP;

		//for test

		if (iResult = listen(s_socket, SOMAXCONN)) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(s_socket);
			WSACleanup();
			return;
		}

		SOCKET so = accept(s_socket, NULL, NULL);
		if (so == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(s_socket);
			WSACleanup();
			return;
		}
		char recvbuf[1024];
		int recvbuflen = 1024;

		do {
			if (iResult = recv(so, recvbuf, recvbuflen, 0)) {
				printf("Bytes received from client: %d\n", iResult);

				//Echo buffer back to sender
				int iSendResult;
				if ((iSendResult = send(so, recvbuf, iResult, 0)) == SOCKET_ERROR) {
					printf("server send failed with error: %d\n", WSAGetLastError());
					closesocket(so);
					WSACleanup();
					return;
				}
				printf("Bytes sent to client: %d\n", iSendResult);
			}
			else if (!iResult) {
				printf("server: connection was closed by client\n");
			}
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(so);
				WSACleanup();
				return;
			}
			
		} while (iResult > 0);

		if (iResult = shutdown(so, SD_SEND)) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(so);
			WSACleanup();
			return;
		}

		closesocket(so);
		WSACleanup();
	}
};

class ClientSocket {
public:

	int err;
	enum SocketStatus socketStatus;

	ClientSocket(const char* ip) :ipaddr(ip){
		c_socket = INVALID_SOCKET;
		err = 0;
		socketStatus = DOWN;
	}
	~ClientSocket() {
		closesocket(c_socket);
		if (clientThread.joinable()) {
			clientThread.join();
		}
	}

	void start() {
		clientThread = std::thread(&ClientSocket::_startClient,this);
	}
	
private:
	SOCKET c_socket;
	WSADATA wsaData;
	std::thread clientThread;
	const char* ipaddr;

	void _startClient() {
		int iResult;

		if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
			printf("WSAStartup failed in client: %d\n", iResult);
			err = 1;
			socketStatus = ERR;
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
			err = 1;
			socketStatus = ERR;
			return;
		}

		c_socket = INVALID_SOCKET;

		//create socket and connect to server
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
				printf("WSAStartup failed in client: %d\n", iResult);
				err = 1;
				socketStatus = ERR;
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
			err = 1;
			socketStatus = ERR;
			return;
		}
		socketStatus = UP;

		//test
		const char* sendbuf = "test message";
		if ((iResult = send(c_socket, sendbuf, (int)strlen(sendbuf), 0)) == SOCKET_ERROR) {
			printf("client send failed with error: %d\n", WSAGetLastError());
			closesocket(c_socket);
			WSACleanup();
			return;
		}

		printf("Bytes sent to server: %ld\n", iResult);

		//shut down connection

		if (iResult = shutdown(c_socket, SD_SEND)) {
			printf("client shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(c_socket);
			WSACleanup();
			return;
		}

		char recvbuf[1024];
		int recvbuflen = 1024;
		do {
			if (iResult = recv(c_socket, recvbuf, recvbuflen, 0)) {
				printf("Bytes received from server: %d\n", iResult);
			}
			else if (iResult == 0) {
				printf("client: connection was closed by server\n");
			}
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
			}
			
		} while (iResult > 0);

		closesocket(c_socket);
		WSACleanup();
	}
};

int main() {

	ServerSocket ss;
	ClientSocket cs("localhost");

	ss.start();
	cs.start();

	return 0;
}
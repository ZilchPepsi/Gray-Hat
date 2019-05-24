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
	}

	std::thread start() {
		return std::thread(&ServerSocket::_startServer,this);
	}

private:
	SOCKET s_socket;
	WSADATA wsaData;

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

		s_socket = INVALID_SOCKET;

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			s_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			if (s_socket != INVALID_SOCKET) {
				if (!(iResult = bind(s_socket, result->ai_addr, (int)result->ai_addrlen))) {
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
		WSACleanup();
	}

	std::thread start() {
		return std::thread(&ClientSocket::_startClient,this, ipaddr);
	}
	
private:
	void _startClient(const char* ipaddr) {
		int iResult;

		//Initialize Winsock
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
			printf("client is INVALID_SOCKET in client\n");
			WSACleanup();
			err = 1;
			socketStatus = ERR;
			return;
		}
		socketStatus = UP;
	}

	SOCKET c_socket;
	WSADATA wsaData;
	const char* ipaddr;
};



int main(char** arc, int argv) {

	printf("I'm running?\n");


	ServerSocket ss;
	ClientSocket cs("localhost");

	ss.start();
	//cs.start();

	printf("DONE\n");

	return 0;
}
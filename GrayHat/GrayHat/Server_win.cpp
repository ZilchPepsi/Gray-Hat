#include "Server_win.h"



Server::Server()
{
	s_socket = INVALID_SOCKET;
	status = DOWN;
	recv_timeout.tv_sec = RECV_TIMEOUT;
	recv_timeout.tv_usec = 0;
	send_timeout.tv_sec = SEND_TIMEOUT;
	send_timeout.tv_usec = 0;
}


Server::~Server()
{
	closesocket(s_socket);
	for (struct Client* c : clients) {
		closesocket(*c->socket);
		delete c->lock;
		delete c;
	}
	WSACleanup();

	if (listenThread.joinable()) {
		listenThread.join();
	}
	if (thread.joinable()) {
		thread.join();
	}
}

void Server::start() {
	thread = std::thread(&Server::_startServer, this);
}

/*
	main server thread
*/
void Server::_startServer() {
	printf("in _startServer\n");
	int iResult;
	if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("WSAStartup failed in server: %d\n", iResult);
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
	hints.ai_flags = AI_PASSIVE;

	if (iResult = getaddrinfo(NULL, PORT, &hints, &result)) {
		printf("getaddrinfo failed in server: %d\n", iResult);
		WSACleanup();
		status = ERR;
		return;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) {
			printf("WSAStartup failed in server: %d\n", iResult);
			status = ERR;
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
		status = ERR;
		return;
	}

	if (iResult = listen(s_socket, SOMAXCONN)) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(s_socket);
		WSACleanup();
		return;
	}


	status = OK;
	listenThread = std::thread(&Server::_listen, this);

	while (status == OK) {

		clientMutex.lock();
		for (int clientPos = 0; clientPos < clientsSize; clientPos++) {

			clients[clientPos]->lock->lock;

			if (setsockopt(*clients[clientPos]->socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_timeout, sizeof(recv_timeout)) < 0) {
				printf("setsockopt rcv_timeout failed: %d\n", WSAGetLastError());
				closesocket(*clients[clientPos]->socket);
				WSACleanup();
				return;
			}
			if (setsockopt(*clients[clientPos]->socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&send_timeout, sizeof(send_timeout)) < 0) {
				printf("setsockopt snd_timeout failed: %d\n", WSAGetLastError());
				closesocket(*clients[clientPos]->socket);
				WSACleanup();
				return;
			}

			do {
				if (iResult = recv(*clients[clientPos]->socket, recvbuf, recvbuflen, 0)) {
					printf("Bytes received from client: %d\n", iResult);
				
					//TODO do something with received bytes from client
				}
				else if (!iResult) {
					printf("server: connection was closed by client\n");
					closesocket(*clients[clientPos]->socket);
					
				}
				else {
					printf("recv failed with error: %d\n", WSAGetLastError());
					closesocket(*clients[clientPos]->socket);
					WSACleanup();
					return;
				}
							
			} while (iResult > 0);


			clients[clientPos]->lock->unlock();
		}
		clientMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}

}

/*
	listener thread
*/
void Server::_listen() {
	while (status == OK) {
		printf("in loop...");
		SOCKET so = accept(s_socket, NULL, NULL);
		if (so == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(s_socket);
			WSACleanup();
			return;
		}
		printf("exiting accept");

		clientMutex.lock();
		if (clientsSize == MAX_CLIENTS) {
			byte message[]{ SERVER_FULL };
			sendBytes(&so, message, 1);
		}
		else {
			clients[clientsSize] = new struct Client(&so, new std::mutex);
			clientsSize++;
		}
		clientMutex.unlock();
	}
}


int Server::sendBytes(const SOCKET* client, const byte* const bytes, int size) const{
	return -1;
}

int Server::getStatus() {
	return status;
}

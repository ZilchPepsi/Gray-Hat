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
	for (struct Client_s* c : clients) {
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

	printf("Server initializing\n");
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

			clients[clientPos]->lock->lock();

			if ((iResult = recv(*clients[clientPos]->socket, recvbuf, recvbuflen, 0))>0) {
					printf("recieved bytes from client: %d\n", iResult);
					//TODO do something with received bytes from client				
			}
			else if (!iResult) {
				printf("server: connection was closed by client\n");
				removeClient(clientPos);
				clientPos--;
					
			}
			else if (WSAGetLastError() != 10060) {
				printf("server receive failed with error: %d\n", WSAGetLastError());
				removeClient(clientPos);
				clientPos--;
			}

			if(clients[clientPos])
				clients[clientPos]->lock->unlock();
		}
		clientMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}
}

int Server::removeClient(int clientPos) {

	delete clients[clientPos];
	for (int x = clientPos + 1; x < clientsSize; x++) {
		clients[x - 1] = clients[x];
	}
	clients[clientsSize - 1] = NULL;
	clientsSize--;

	return 0;
}


/*
	listener thread
*/
void Server::_listen() {
	printf("server listening for connections...\n");
	while (status == OK) {
		SOCKET so = accept(s_socket, NULL, NULL);
		if (so == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(so);
			continue;
		}
		printf("server got a connection\n");

		if (setsockopt(so, SOL_SOCKET, SO_RCVTIMEO, (char*)&recv_timeout, sizeof(recv_timeout)) < 0) {
			printf("setsockopt rcv_timeout failed: %d\n", WSAGetLastError());
			closesocket(so);
			continue;
		}
		if (setsockopt(so, SOL_SOCKET, SO_SNDTIMEO, (char*)&send_timeout, sizeof(send_timeout)) < 0) {
			printf("setsockopt snd_timeout failed: %d\n", WSAGetLastError());
			closesocket(so);
			continue;
		}

		clientMutex.lock();
		if (clientsSize == MAX_CLIENTS) {
			clientMutex.unlock();
			char message[]{ SERVER_FULL };
			sendBytes(&so, message, 1);
		}
		else {
			clients[clientsSize] = new struct Client_s(&so, new std::mutex);
			clientsSize++;
		}
		clientMutex.unlock();
	}
}


int Server::sendBytes(struct Client_s* client, const char* const bytes, int size) const{
	return sendBytes(client->socket, bytes, size);
}

int Server::sendBytes(SOCKET* client, const char* const bytes, int size) const {
	int iSendResult;
	if ((iSendResult = send(*client, bytes, size, 0)) == SOCKET_ERROR) {
		printf("server send failed with error: %d\n", WSAGetLastError());
		return WSAGetLastError();
	}
	printf("Bytes sent to client: %d\n", iSendResult);
	return 0;
}

int Server::getStatus() {
	return status;
}

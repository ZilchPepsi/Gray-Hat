#pragma once

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")


class Client
{
public:
	Client();
	~Client();
};


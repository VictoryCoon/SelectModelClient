#pragma once
#include <string>
#include "RingBuffer.h"

using namespace std;

class Session
{
public:
	int id;
	SOCKET socket;
	sockaddr_in address;
	RingBuffer recvBuffer;
	RingBuffer sendBuffer;
	char ipAddress[INET_ADDRSTRLEN];
	unsigned long port;
	bool sendFlag = false;

	Session(int id, SOCKET socket, sockaddr_in address)
	{
		this->id = id;
		this->socket = socket;
		this->address = address;

		inet_ntop(AF_INET, &address.sin_addr, ipAddress, INET_ADDRSTRLEN);
		port = ntohs(address.sin_port);
	}
};
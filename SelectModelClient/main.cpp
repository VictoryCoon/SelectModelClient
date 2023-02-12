#include "PreCompliedHeader.h"

char* serverIP = (char*)"127.0.0.1";
#define SERVERPORT 16000
#define BUFSIZE 1024

ErrCommon ec;

void ProcessResponseChat(Packet* packet);

void OnReceived(Packet* packet)
{
	MsgType type;
	packet->Read(&type);

	switch (type)
	{
	case ResponseChat:
		ProcessResponseChat(packet);
		break;
	default:
		break;
	}
}

void ProcessResponseChat(Packet* packet)
{
	MsgResponseChat msg;
	*packet >> msg.chatLength;
	packet->Read(msg.chatLength, &msg.chatMsg);
	delete packet;

	cout << msg.chatMsg << endl;
}

int main()
{
	int returnValue;
	unsigned long on = 1;

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
	{
		return 1;
	}

	printf("[Alert] Window Socket Initializing is success");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		ec.ErrorQuit("Socket()");
	}

	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, serverIP, &serverAddr.sin_addr);

	returnValue = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (returnValue == SOCKET_ERROR)
	{
		ec.ErrorQuit("Connect()");
	}

	returnValue = ioctlsocket(sock, FIONBIO, &on);
	if (returnValue == SOCKET_ERROR)
	{
		ec.ErrorDisplay("IoctlSocket()");
	}

	int len;
	char buf[BUFSIZE + 1];
	RingBuffer recvBuffer;
	RingBuffer sendBuffer;
	fd_set rSet;
	fd_set wSet;
	timeval time;
	time.tv_sec = 0;
	time.tv_usec = 0;
	bool sendFlag = false;

	while (1)
	{
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		FD_SET(sock, &rSet);
		if (sendFlag)
		{
			sendFlag = false;
			FD_SET(sock, &wSet);
		}

		returnValue = select(0, &rSet, &wSet, NULL, &time);
		if (returnValue == SOCKET_ERROR)
		{
			ec.ErrorQuit("select()");
		}

		if (_kbhit())
		{
			memset(buf, 0, BUFSIZE + 1);
			printf("[Àü¼Û] ");
			if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			{
				break;
			}
			
			MsgRequestChat msg;
			msg.type = MsgType::RequestChat;
			msg.chatLength = strlen(buf);
			msg.chatMsg = buf;
			Packet* sendPacket = new Packet();
			sendPacket->Write(msg.type);
			sendPacket->Write(msg.chatLength);
			sendPacket->Write(msg.chatMsg);

			Header header;
			header.magicNumber = MagicNumber;
			header.payloadLength = sendPacket->Count();

			sendPacket->SetHeader(header);

			sendBuffer.Write(sendPacket->GetBuffer(), sendPacket->Count());
			sendFlag = true;
		}

		if (FD_ISSET(sock, &rSet))
		{
			returnValue = recv(sock, recvBuffer.GetWriteBuffer(), recvBuffer.GetWritableSizeAtOnce(),0);
			if(returnValue == SOCKET_ERROR)
			{
				ec.ErrorDisplay("Recv()");
			}
			else if (returnValue == 0)
			{
				break;
			}

			recvBuffer.MoveWPos(returnValue);

			Header header;
			int headerSize = sizeof(header);
			
			while (1)
			{
				int bufferSize = recvBuffer.GetReadableSize();
				if (bufferSize < headerSize)
				{
					break;
				}
				recvBuffer.Peek((char*)&header, headerSize);
				if (header.magicNumber != MagicNumber)
				{
					ec.ErrorQuit("MagicNumber()");
					break;
				}

				if (bufferSize < headerSize + header.payloadLength)
				{
					break;
				}
				Packet* recvPacket = new Packet();
				recvBuffer.MoveRPos(headerSize);
				recvBuffer.Read(recvPacket->GetBuffer(), header.payloadLength);
				recvPacket->MoveWPos(header.payloadLength);
				OnReceived(recvPacket);
			}


		}

		if (FD_ISSET(sock, &wSet))
		{
			char* buf = sendBuffer.GetReadBuffer();
			int length = sendBuffer.GetReadableSizeAtOnce();
			returnValue = send(sock, buf, length, 0);
			if (returnValue == SOCKET_ERROR)
			{
				ec.ErrorDisplay("Send()");
				break;
			}

			sendBuffer.MoveRPos(returnValue);
		}
	}

	closesocket(sock);
	
	WSACleanup();
	
	return 0;
}
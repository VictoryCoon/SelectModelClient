#include "PreCompliedHeader.h"
#pragma once
#define MagicNumber 0x77

enum MsgType
{
	RequestChat = 100,
	ResponseChat,
};

#pragma pack(push,1)
struct Header
{
	char magicNumber = MagicNumber;
	int payloadLength;
	char checkSum;
};

#pragma pack(pop)

struct MsgRequestChat
{
	MsgType type = MsgType::RequestChat;
	int chatLength;
	string chatMsg;
};

struct MsgResponseChat
{
	MsgType type = MsgType::ResponseChat;
	int chatLength;
	string chatMsg;
};
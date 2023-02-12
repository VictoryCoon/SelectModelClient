#pragma once
#include "PreCompliedHeader.h"
#include "Packet.h"

Packet::Packet()
{
	buffer = new char[defaultMemorySize];
	memSize = defaultMemorySize;
	w = 0;
	r = 0;
}

Packet::Packet(int size)
{
	buffer = new char[size];
	memSize = size;
	w = 0;
	r = 0;
}

Packet::~Packet()
{
	delete[] buffer;
}

Packet& Packet::operator<<(char rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(short rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(unsigned short rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(int rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(unsigned int rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(long rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(unsigned long rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(long long rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(unsigned long long rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(float rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator<<(double rhs)
{
	Write(rhs);
	return *this;
}

Packet& Packet::operator>>(char& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(short& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(unsigned short& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(int& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(unsigned int& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(long& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(unsigned long& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(long long& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(unsigned long long& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(float& rhs)
{
	Read(&rhs);
	return *this;
}

Packet& Packet::operator>>(double& rhs)
{
	Read(&rhs);
	return *this;
}

void Packet::Write(string str)
{
	int size = str.length();

	if (w + size >= memSize)
	{
		char* newBuffer = new char[w + size];
		memcpy(newBuffer, buffer, Count());
		delete[] buffer;
		buffer = newBuffer;
	}
	memcpy(&buffer[w], &str[0], size);
	w += size;
}

bool Packet::Read(int byteSize, string* outStr)
{
	int size = byteSize;
	if (Count() < size)
	{
		return false;
	}

	char* data = new char[size + 1];
	memcpy(data, &buffer[r], size + 1);
	data[size] = '\0';

	if (outStr != nullptr)
	{
		*outStr = data;
	}

	r += size;

	return true;
}

int Packet::Count() const
{
	return w - r;
}

char* Packet::GetBuffer() const
{
	return buffer;
}

void Packet::MoveWPos(int count)
{
	w += count;
}

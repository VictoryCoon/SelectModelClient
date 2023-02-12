#pragma once
#include <iostream>
#include <string>

using namespace std;

class Packet
{
public:
	Packet();
	Packet(int size);
	~Packet();

	Packet& operator<<(char rhs);
	Packet& operator<<(short rhs);
	Packet& operator<<(unsigned short rhs);
	Packet& operator<<(int rhs);
	Packet& operator<<(unsigned int rhs);
	Packet& operator<<(long rhs);
	Packet& operator<<(unsigned long rhs);
	Packet& operator<<(long long rhs);
	Packet& operator<<(unsigned long long rhs);
	Packet& operator<<(float rhs);
	Packet& operator<<(double rhs);

	Packet& operator>>(char& rhs);
	Packet& operator>>(short& rhs);
	Packet& operator>>(unsigned short& rhs);
	Packet& operator>>(int& rhs);
	Packet& operator>>(unsigned int& rhs);
	Packet& operator>>(long& rhs);
	Packet& operator>>(unsigned long& rhs);
	Packet& operator>>(long long& rhs);
	Packet& operator>>(unsigned long long& rhs);
	Packet& operator>>(float& rhs);
	Packet& operator>>(double& rhs);

	template<class T>
	void Write(T data);

	void Write(string str);

	template<class T>
	bool Read(T* outData);

	bool Read(int byteSize, string* outStr);

	template<class T>
	bool Peek(T* outData);

	template<class T>
	void SetHeader(T header);

	int Count() const;

	char* GetBuffer() const;

	void MoveWPos(int count);

private:
	char* buffer;
	int w;
	int r;
	int memSize;
	const int defaultMemorySize = 1024;
};

template<class T>
inline void Packet::Write(T data)
{
	int size = sizeof(T);
	if (w + size >= memSize)
	{
		char* newBuffer = new char[w + size];
		memcpy(newBuffer, buffer, Count());
		delete[] buffer;
		buffer = newBuffer;
	}
	memcpy(&buffer[w], &data, size);
	w += size;
}

template<class T>
inline bool Packet::Read(T* outData)
{
	T data;
	int size = sizeof(T);
	if (Count() < size)
	{
		return false;
	}

	memcpy(&data, &buffer[r], size);

	if (outData != nullptr)
	{
		*outData = data;
	}

	r += size;

	return true;
}

template<class T>
inline bool Packet::Peek(T* outData)
{
	T data;
	int size = sizeof(T);
	if (Count() < size)
	{
		return false;
	}

	memcpy(&data, &buffer[r], size);

	if (outData != nullptr)
	{
		*outData = data;
	}

	return true;
}

template<class T>
inline void Packet::SetHeader(T header)	// Write to front
{
	int size = sizeof(T);
	if (w + size >= memSize)
	{
		char* newBuffer = new char[w + size];
		memcpy(newBuffer, buffer, Count());
		delete[] buffer;
		buffer = newBuffer;
	}
	memcpy(buffer + size, buffer, Count());
	memcpy(&buffer[0], &header, size);
	w += size;
}
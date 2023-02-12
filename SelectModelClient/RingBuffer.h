#pragma once

class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(int size);
	~RingBuffer();

	int Write(char* src, int count);
	int Read(char* dst, int count);
	int Peek(char* dst, int count);

	int GetReadableSize() const;
	int GetReadableSizeAtOnce() const;

	int GetWritableSize() const;
	int GetWritableSizeAtOnce() const;

	char* GetReadBuffer();
	char* GetWriteBuffer();

	void MoveWPos(int count);
	void MoveRPos(int count);

	bool IsEmpty();

	void Print();
public:
	const int DefaultMemorySize = 1024;
private:
	char* buffer;
	int rPos;
	int wPos;
	int memSize;
};
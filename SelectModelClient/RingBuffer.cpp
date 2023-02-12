#include "PreCompliedHeader.h"

RingBuffer::RingBuffer()
{
	buffer = new char[DefaultMemorySize];
	memSize = DefaultMemorySize;
	rPos = 0;
	wPos = 0;
}

RingBuffer::RingBuffer(int size)
{
	buffer = new char[size];
	memSize = size;
	rPos = 0;
	wPos = 0;
}

RingBuffer::~RingBuffer()
{
	delete[] buffer;
}

int RingBuffer::Write(char* src, int count)
{
	int writableSize = GetWritableSize();
	int writableOnce = GetWritableSizeAtOnce();
	int sizeToWrite = count < writableSize ? count : writableSize;
	int once = sizeToWrite < writableOnce ? sizeToWrite : writableOnce;
	int remain = sizeToWrite - once;

	memcpy(buffer + wPos, src, once);
	MoveWPos(once);
	if (remain > 0)
	{
		memcpy(buffer + wPos, src + remain, remain);
		MoveWPos(remain);
	}
	return sizeToWrite;
}

int RingBuffer::Read(char* dst, int count)
{
	int readableSize = GetReadableSize();
	int readableOnce = GetReadableSizeAtOnce();
	int sizeToRead = count < readableSize ? count : readableSize;
	int once = sizeToRead < readableOnce ? sizeToRead : readableOnce;
	int remain = sizeToRead - once;

	memcpy(dst, buffer + rPos, once);
	MoveRPos(once);
	if (remain > 0)
	{
		memcpy(dst + once, buffer + rPos, remain);
		MoveRPos(remain);
	}

	return sizeToRead;
}

int RingBuffer::Peek(char* dst, int count)
{
	int readableSize = GetReadableSize();
	int readableOnce = GetReadableSizeAtOnce();
	int sizeToRead = count < readableSize ? count : readableSize;
	int once = sizeToRead < readableOnce ? sizeToRead : readableOnce;
	int remain = sizeToRead - once;

	memcpy(dst, buffer + rPos, once);

	if (remain > 0)
	{
		int readPos = (rPos + once) % memSize;
		memcpy(dst + once, buffer + readPos, remain);
	}

	return sizeToRead;
}

int RingBuffer::GetReadableSize() const
{
	return rPos <= wPos ? wPos - rPos : memSize - rPos + wPos;;
}

int RingBuffer::GetReadableSizeAtOnce() const
{
	int readableSize = GetReadableSize();
	int readToEnd = memSize - rPos;
	int sizeOnce = readToEnd < readableSize ? readToEnd : readableSize;	//둘 중 작은 것
	return sizeOnce;
}

int RingBuffer::GetWritableSize() const
{
	return memSize - GetReadableSize() - 1;
}

int RingBuffer::GetWritableSizeAtOnce() const
{
	int writableSize = GetWritableSize();
	int writeToEnd = memSize - wPos;
	int sizeOnce = writeToEnd < writableSize ? writeToEnd : writableSize;	//둘 중 작은 것
	return sizeOnce;
}

char* RingBuffer::GetReadBuffer()
{
	return &buffer[rPos];
}

char* RingBuffer::GetWriteBuffer()
{
	return &buffer[wPos];
}

void RingBuffer::MoveWPos(int count)
{
	wPos += count;
	wPos = wPos % memSize;
}

void RingBuffer::MoveRPos(int count)
{
	rPos += count;
	rPos = rPos % memSize;
}

bool RingBuffer::IsEmpty()
{
	return rPos == wPos;
}

void RingBuffer::Print()
{
	std::cout << "Read: " << rPos << "/ Write: " << wPos << "/ Size: " << GetReadableSize();
	std::cout << std::endl;
	std::cout << "[";
	for (int i = 0; i < memSize; i++)
	{
		if (i == rPos)
		{
			std::cout << "(";
		}
		if (i == wPos)
		{
			std::cout << "<";
		}
		std::cout << (int)buffer[i];
		if (i == wPos)
		{
			std::cout << ">";
		}
		if (i == rPos)
		{
			std::cout << ")";
		}
		std::cout << " ";
	}
	std::cout << "]" << std::endl;
}
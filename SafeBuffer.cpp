
#include "SafeBuffer.h"

SafeBuffer::SafeBuffer(byte size)
{
    this->buffer = new byte[size + 1];
    this->bufferSize = size;

    memset(this->buffer, 0, this->bufferSize + 1);
}

SafeBuffer::~SafeBuffer()
{
    this->wipe();
    delete this->buffer;
}

byte SafeBuffer::getBufferSize()
{
    return this->bufferSize;
}

char *SafeBuffer::getBuffer()
{
    return this->buffer;
}

byte SafeBuffer::strlen()
{
    return strnlen(this->buffer, this->bufferSize);
}

void SafeBuffer::setChar(byte position, char ch)
{
    if (position >= this->bufferSize)
    {
        return;
    }

    this->buffer[position] = ch;
}

char SafeBuffer::getChar(byte position)
{
    if (position >= this->bufferSize)
    {
        return;
    }

    return this->buffer[position];
}

void SafeBuffer::strcpy(char *src)
{
    strncpy(this->buffer, src, this->bufferSize);
}

void SafeBuffer::wipe()
{
    memset(this->buffer, 0xFF, this->bufferSize);
    memset(this->buffer, 0, this->bufferSize);
}
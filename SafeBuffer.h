
#ifndef __SAFE_BUFFER_H__
#define __SAFE_BUFFER_H__

#include <Arduino.h>

class SafeBuffer
{
private:
    char *buffer;
    byte bufferSize;

public:
    SafeBuffer(byte size);    
    ~SafeBuffer();

    byte getBufferSize();
    char *getBuffer();
    byte strlen();
    void strcpy(char *src);
    void setChar(byte position, char ch);
    char getChar(byte position);
    void wipe();
};

#endif
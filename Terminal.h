#ifndef ___PGEN_TERMINAL_H__
#define ___PGEN_TERMINAL_H__

#include "VT100.h"
#include "messages.h"

class Terminal
{
private:
    Stream *stream;
    void printMessage(uint8_t messageId);

public:
    void init(Stream *stream);
    void clearScreen();
    void printBanner();
    void printStatusMessage(char *message);    
    void printMasterPasswordPrompt();
    void readMasterPassword(char *masterPassword, byte masterPasswordMaxSize);
};

#endif
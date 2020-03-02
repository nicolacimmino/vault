#ifndef ___PGEN_TERMINAL_H__
#define ___PGEN_TERMINAL_H__

#include "VT100.h"
#include "messages.h"

#define TERMINAL_MAX_SHORTCUTS 4
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGTH 24
#define TERMINAL_FIRST_CANVAS_LINE 10
#define TERMINAL_CANVAS_LINES 12
#define TERMINAL_STATUS_LINE 23
#define TERMINAL_BACKGROUND_COLOR VT_BLACK
#define TERMINAL_FOREGROUND_COLOR VT_YELLOW
#define TERMINAL_STATUS_LINE_BACKGROUND_COLOR VT_YELLOW
#define TERMINAL_STATUS_LINE_FOREGROUND_COLOR VT_BLACK
#define TERMINAL_MENU_BASE 100

class Terminal
{
private:
    Stream *stream;
    void printMessage(uint8_t messageId);
    struct terminalShortcut
    {
        byte contextId;
        char key;
    };

    terminalShortcut terminalShortcuts[TERMINAL_MAX_SHORTCUTS];

public:
    void init(Stream *stream);
    void setShortcut(byte index, char key, byte contextId);
    void clearScreen();
    void clearCanvas();
    void printBanner();
    void printStatusMessage(char *message);
    void print(char *text, byte line = NULL, byte column = NULL);
    void printMenuEntry(byte position, char *text);
    void readString(char *string, byte stringMaxSize, char mask = 0);
    byte waitMenuSelection();
    
};

#endif
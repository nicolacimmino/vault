#ifndef ___PGEN_TERMINAL_H__
#define ___PGEN_TERMINAL_H__

#include "VT100.h"
#include "messages.h"

#define TERMINAL_MAX_HOTKEYS 4
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGTH 24
#define TERMINAL_FIRST_CANVAS_LINE 10
#define TERMINAL_CANVAS_LINES 12
#define TERMINAL_STATUS_LINE 23
#define TERMINAL_HEADER_LINE 1
#define TERMINAL_BANNER_LINE 2
#define TERMINAL_BACKGROUND_COLOR VT_BLACK
#define TERMINAL_FOREGROUND_COLOR VT_YELLOW
#define TERMINAL_LABEL_COLOR VT_WHITE
#define TERMINAL_STATUS_LINE_BACKGROUND_COLOR VT_YELLOW
#define TERMINAL_STATUS_LINE_FOREGROUND_COLOR VT_BLACK
#define TERMINAL_MENU_BASE 100
#define TERMINAL_MAX_INACTIVE_TIME_MS 60000

class Terminal
{
private:
    void printMessage(uint8_t messageId);
    void printHeader();
    void resetInactivityTimer();
    int getFreeRamBytes();

    struct terminalHotkey
    {
        void (*callback)();
        char key;
    };

    Stream *stream;
    terminalHotkey hotkeys[TERMINAL_MAX_HOTKEYS];
    byte lastHotkeyIndex = 0;
    void (*menuCallback)(byte selection);
    void (*resetCallback)();
    uint32_t lastActiveTime;
    bool clpIndicator = false;
    bool lckIndicator = false;

public:
    void init(Stream *stream);
    void clearHotkeys();
    void addHotkey(char key, void (*callback)());
    void setMenuCallback(void (*menuCallback)(byte selection));
    void setResetCallback(void (*resetCallback)());
    void clearScreen();
    void clearCanvas();
    void printBanner();
    void printHeaderMessage(char *message);
    void printStatusMessage(char *message);
    void printStatusProgress(char *message, uint32_t delaymS, byte *completedMessage, byte line, byte column, byte areaWidth);
    void print(char *text, byte line = NULL, byte column = NULL);
    void printMenuEntry(byte position, char *text);
    void readString(char *prompt, char *string, byte stringMaxSize, char mask = 0, byte line = NULL, byte column = NULL);
    void setClpIndicator(bool status);
    void setLclIndicator(bool status);
    void loop();
};

#endif
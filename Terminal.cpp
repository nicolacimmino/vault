
#include "Terminal.h"

void Terminal::init(Stream *stream)
{
    this->stream = stream;
    VT100.begin(*stream);
    this->lastActiveTime = millis();
}

void Terminal::resetInactivityTimer()
{
    this->lastActiveTime = millis();
}

void Terminal::clearHotkeys()
{
    this->lastHotkeyIndex = 0;
}

void Terminal::addHotkey(char key, void (*callback)())
{
    if (this->lastHotkeyIndex == TERMINAL_MAX_HOTKEYS)
    {
        return;
    }

    terminalHotkey hotkey;
    hotkey.callback = callback;
    hotkey.key = key;

    this->hotkeys[this->lastHotkeyIndex] = hotkey;

    this->lastHotkeyIndex++;
}

void Terminal::setMenuCallback(void (*menuCallback)(byte selection))
{
    this->menuCallback = menuCallback;
}

void Terminal::setResetCallback(void (*resetCallback)())
{
    this->resetCallback = resetCallback;
}

void Terminal::loop()
{
    static bool alt = false;
    
    while (Serial.available())
    {
        this->lastActiveTime = millis();
           
        char key = this->stream->read();

        if (key == (char)27)
        {
            alt = true;
            continue;
        }

        if (alt)
        {            
            if (key == 'q' && this->resetCallback)
            {
                this->resetCallback();
            }

            for (byte ix = 0; ix < this->lastHotkeyIndex; ix++)
            {
                if (key == this->hotkeys[ix].key)
                {
                    this->hotkeys[ix].callback();
                }
            }
        }

        if (!alt && key >= 'a' && key <= 'z' && this->menuCallback)
        {
            this->menuCallback(key - 'a');
        }

        alt = false;
    }

    if(millis() - this->lastActiveTime > TERMINAL_MAX_INACTIVE_TIME_MS && this->resetCallback) {
        this->resetCallback();
    }
}

void Terminal::clearScreen()
{
    VT100.setBackgroundColor(TERMINAL_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_FOREGROUND_COLOR);
    VT100.clearScreen();
    this->printStatusMessage("");
}

void Terminal::clearCanvas()
{
    VT100.setBackgroundColor(TERMINAL_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_FOREGROUND_COLOR);
    for (byte line = 0; line < TERMINAL_CANVAS_LINES; line++)
    {
        VT100.setCursor(line + TERMINAL_FIRST_CANVAS_LINE, 1);
        VT100.clearLineAfter();
    }
}

void Terminal::printBanner()
{
    VT100.setCursor(TERMINAL_BANNER_LINE, 1);
    this->printMessage(0);
}

void Terminal::printStatusMessage(char *message)
{
    VT100.setCursor(TERMINAL_STATUS_LINE, 1);
    VT100.setBackgroundColor(TERMINAL_STATUS_LINE_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_STATUS_LINE_FOREGROUND_COLOR);
    this->stream->print(message);
    VT100.clearLineAfter();
    VT100.setCursor(TERMINAL_STATUS_LINE, TERMINAL_WIDTH);
    VT100.setBackgroundColor(TERMINAL_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_FOREGROUND_COLOR);
    VT100.clearLineAfter();
}

void Terminal::printHeaderMessage(char *message)
{
    VT100.setCursor(TERMINAL_HEADER_LINE, 1);
    VT100.setBackgroundColor(TERMINAL_STATUS_LINE_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_STATUS_LINE_FOREGROUND_COLOR);
    this->stream->print(message);
    VT100.clearLineAfter();
    VT100.setCursor(TERMINAL_HEADER_LINE, TERMINAL_WIDTH);
    VT100.setBackgroundColor(TERMINAL_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_FOREGROUND_COLOR);
    VT100.clearLineAfter();
}

void Terminal::print(char *text, byte line = NULL, byte column = NULL)
{
    if (line && column)
    {
        VT100.setCursor(line, column);
    }

    this->stream->print(text);
}

void Terminal::printMenuEntry(byte position, char *text)
{
    byte line = (position % TERMINAL_CANVAS_LINES) + TERMINAL_FIRST_CANVAS_LINE;
    byte column = (position < TERMINAL_CANVAS_LINES) ? 2 : 22;

    char buffer[TERMINAL_WIDTH];
    sprintf(buffer, "   %s[%c]%s  %s", VT_FOREGROUND_WHITE, 'A' + position, VT_FOREGROUND_YELLOW, text);
    buffer[TERMINAL_WIDTH - 1] = 0;
    this->print(buffer, line, column);
}

void Terminal::readString(char *prompt, char *string, byte stringMaxSize, char mask = 0, byte line = NULL, byte column = NULL)
{
    this->print(prompt, line, column);

    VT100.cursorOn();
    byte ix = 0;
    while (true)
    {
        if (Serial.available())
        {            
            char nextChar = this->stream->read();

            if (nextChar == '\r')
            {
                string[ix] = 0;
                break;
            }

            string[ix] = nextChar;
            ix++;

            if (ix == stringMaxSize - 1)
            {
                break;
            }

            this->stream->print(mask ? mask : nextChar);
        }
    }

    VT100.cursorOff();
}

void Terminal::printMessage(uint8_t messageId)
{
    char buffer[512];
    strcpy_P(buffer, (char *)pgm_read_word(&(messages[messageId])));
    this->stream->print(buffer);
}

#include "Terminal.h"

void Terminal::init(Stream *stream)
{
    this->stream = stream;
    VT100.begin(*stream);
}

void Terminal::clearScreen()
{
    VT100.setBackgroundColor(VT_BLACK);
    VT100.clearScreen();
    this->printStatusMessage("");
}

void Terminal::printBanner()
{
    VT100.setCursor(1, 1);
    this->printMessage(0);
}

void Terminal::printStatusMessage(char *message)
{
    VT100.setCursor(12, 1);
    VT100.setBackgroundColor(VT_YELLOW);
    VT100.setTextColor(VT_BLACK);
    this->stream->print(message);
    VT100.clearLineAfter();
    VT100.setBackgroundColor(VT_BLACK);
    VT100.setTextColor(VT_YELLOW);
    VT100.setCursor(12, 30);
    VT100.clearLineAfter();
}

void Terminal::printMasterPasswordPrompt()
{
    VT100.setCursor(9, 1);
    this->stream->print("Enter Master Password");
}

void Terminal::readMasterPassword(char *masterPassword, byte masterPasswordMaxSize)
{
    VT100.setCursor(10, 1);
    byte ix = 0;
    while (true)
    {
        if (Serial.available())
        {
            char nextChar = this->stream->read();

            if (nextChar == '\r')
            {
                masterPassword[ix] = 0;
                break;
            }

            masterPassword[ix] = nextChar;
            ix++;

            if (ix == masterPasswordMaxSize - 1)
            {
                break;
            }

            this->stream->print("*");
        }
    }
}

void Terminal::printMessage(uint8_t messageId)
{
    char buffer[256];
    strcpy_P(buffer, (char *)pgm_read_word(&(messages[messageId])));
    this->stream->print(buffer);
}
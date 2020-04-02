
#include "Terminal.h"

Terminal::Terminal(EncryptedStore *encryptedStore)
{
    this->encryptedStore = encryptedStore;
}

void Terminal::init()
{
    Serial.begin(TERMINAL_SERIAL_SPEED);

    while (!Serial)
    {
        ;
    }
    delay(500);

    this->stream = &Serial;
    VT100.begin(*this->stream);
    this->lastActiveTime = millis();
}

void Terminal::resetInactivityTimer()
{
    this->lastActiveTime = millis();
}

bool Terminal::checkInactivityTimer()
{
    if (millis() - this->lastActiveTime > TERMINAL_MAX_INACTIVE_TIME_MS && this->resetCallback)
    {
        this->resetCallback();

        return false;
    }

    return true;
}

void Terminal::clearHotkeys()
{
    this->lastHotkeyIndex = 0;
}

void Terminal::addHotkey(char key, const Functor0 &action)
{
    if (this->lastHotkeyIndex == TERMINAL_MAX_HOTKEYS)
    {
        return;
    }

    terminalHotkey hotkey;
    hotkey.callback = action;
    hotkey.key = key;

    this->hotkeys[this->lastHotkeyIndex] = hotkey;

    this->lastHotkeyIndex++;
}

void Terminal::setMenuCallback(const Functor1<byte> &menuCallback)
{
    this->menuCallback = menuCallback;
}

void Terminal::setResetCallback(const Functor0 &resetCallback)
{
    this->resetCallback = resetCallback;
}

void Terminal::clearInputBuffer()
{
    while (this->stream->available())
    {
        this->stream->read();
    }
}

void Terminal::loop(bool noTerminalInput = false)
{
    if (millis() % 300 == 0)
    {
        this->printHeader();
    }

    this->checkInactivityTimer();

    if (noTerminalInput)
    {
        return;
    }

    static bool alt = false;

    while (this->stream->available())
    {
        this->resetInactivityTimer();

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

            alt = false;

            continue;
        }

        if (key >= 'a' && (key <= 'a' + this->maxMenuPosition))
        {
            byte menuIndex = key - 'a';
            this->highlightMenuEntry(menuIndex);

            if (this->menuCallback)
            {
                this->menuCallback(menuIndex);
            }

            if (this->menuCallbacks[menuIndex])
            {
                this->menuCallbacks[menuIndex]();
            }
        }
    }
}

void Terminal::clearScreen()
{
    VT100.setBackgroundColor(TERMINAL_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_FOREGROUND_COLOR);
    VT100.clearScreen();
    this->printHeader();
    this->printBanner();
    this->printStatusMessage("");
    this->maxMenuPosition = 0;
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
    this->maxMenuPosition = 0;
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
    VT100.setCursor(TERMINAL_STATUS_LINE, TERMINAL_WIDTH + 1);
    VT100.setBackgroundColor(TERMINAL_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_FOREGROUND_COLOR);
    VT100.clearLineAfter();
}

void Terminal::alert(char *message, bool warning = false)
{
    VT100.setCursor(TERMINAL_FIRST_CANVAS_LINE + 1, 1);
    this->printMessage(1);

    if (warning)
    {
        VT100.setCursor(TERMINAL_FIRST_CANVAS_LINE + 2, (TERMINAL_WIDTH - strlen(TXT_WARNING)) / 2);
        this->stream->print(TXT_WARNING);
    }

    VT100.setCursor(TERMINAL_FIRST_CANVAS_LINE + 3, (TERMINAL_WIDTH - strlen(message)) / 2);
    this->stream->print(message);
}

void Terminal::print(char *text, byte line = NULL, byte column = NULL)
{
    if (line && column)
    {
        VT100.setCursor(line, column);
    }

    this->stream->print(text);
}

void Terminal::printMenuEntry(byte position, char *text, char *selectorColor = VT_FOREGROUND_WHITE, Functor0 callback = NULL)
{
    this->menuCallbacks[position] = callback;

    this->maxMenuPosition = max(this->maxMenuPosition, position);

    byte line = (position % TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION) + TERMINAL_FIRST_CANVAS_LINE;
    byte column = (position < TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION) ? TERMINAL_LEFT_MENU_FIRST_COLUMN : TERMINAL_RIGHT_MENU_FIRST_COLUMN;

    char buffer[TERMINAL_WIDTH];
    sprintf(buffer, "%s[%c]%s  %s", selectorColor, 'A' + position, VT_FOREGROUND_YELLOW, text);
    buffer[TERMINAL_WIDTH - 1] = 0;
    this->print(buffer, line, column);
}

void Terminal::highlightMenuEntry(byte position)
{
    this->printMenuEntry(position, "", VT_FOREGROUND_GREEN, this->menuCallbacks[position]);
}

void Terminal::flowControl(bool on)
{
    Serial.write(on ? TERMINAL_XON : TERMINAL_XOFF);
    Serial.flush();
}

bool Terminal::clientRequestedAbort()
{
    if (this->stream->peek() == TERMINAL_KEY_ESC)
    {
        this->stream->read();
        return true;
    }

    return false;
}

void Terminal::readLine(char *line, byte bufferSize)
{
    while (!Serial.available())
    {
        delay(1);
    }

    size_t bytesRead = Serial.readBytesUntil('\r', line, bufferSize);

    line[bytesRead] = 0;
}

bool Terminal::askQuestion(char *prompt, char *string, byte stringMaxSize, char mask = 0, byte line = NULL, byte column = NULL, bool noTimeout = false)
{
    this->print(prompt, line, column);

    VT100.cursorOn();
    byte ix = 0;
    bool alt = false;
    while (true)
    {
        if (this->stream->available())
        {
            this->resetInactivityTimer();

            char nextChar = this->stream->read();

            if (nextChar == (char)27)
            {
                alt = true;
                continue;
            }

            if (alt)
            {
                if (nextChar == 'q' && this->resetCallback)
                {
                    this->resetCallback();

                    return false;
                }

                alt = false;
            }

            if (nextChar == '\r')
            {
                string[ix] = 0;
                break;
            }

            if (nextChar == TERMINAL_KEY_BACKSPACE)
            {
                if (ix > 0)
                {
                    ix--;
                    this->stream->print(nextChar);
                }
                continue;
            }

            if (ix == stringMaxSize - 1)
            {
                continue;
            }

            string[ix] = nextChar;
            ix++;

            this->stream->print(mask ? mask : nextChar);
        }

        if (!noTimeout && !this->checkInactivityTimer())
        {
            return false;
        }
    }

    VT100.cursorOff();

    return true;
}

bool Terminal::askYesNoQuestion(char *question, bool warning = false)
{
    this->alert(question, warning);

    while (true)
    {
        byte key = this->waitKeySelection();

        if (key != 'n' && key != 'y' && key != TERMINAL_OPERATION_ABORTED)
        {
            continue;
        }

        return key == 'y';
    }
}

char Terminal::waitKeySelection()
{
    while (true)
    {
        if (!this->checkInactivityTimer() || this->clientRequestedAbort())
        {
            return TERMINAL_OPERATION_ABORTED;
        }

        if (!this->stream->available())
        {
            continue;
        }

        this->resetInactivityTimer();

        return this->stream->read();
    }
}

void Terminal::printMessage(uint8_t messageId)
{
    char buffer[512];
    strcpy_P(buffer, (char *)pgm_read_word(&(messages[messageId])));
    this->stream->print(buffer);
}

void Terminal::printHeader()
{
    char headerMessage[TERMINAL_WIDTH];
    memset(headerMessage, 0, TERMINAL_WIDTH);

    if (this->encryptedStore->isLocked())
    {
        sprintf(headerMessage, TXT_LOCKED_TERMINAL_HEADER_PROTOTYPE, this->getFreeRamBytes());
    }
    else
    {
        if (millis() < this->showKeyFingerprintUntil)
        {
            sprintf(headerMessage, TXT_UNLOCKED_TERMINAL_HEADER_PROTOTYPE,
                    NoiseSource::instance()->getRandomPoolFillStatus(),
                    this->encryptedStore->getKeyFingerprint(),
                    this->getFreeRamBytes());
        }
        else
        {
            sprintf(headerMessage, TXT_UNLOCKED_TERMINAL_HEADER_PROTOTYPE_NOKFP,
                    NoiseSource::instance()->getRandomPoolFillStatus(),
                    this->getFreeRamBytes());
        }
    }

    VT100.setCursor(TERMINAL_HEADER_LINE, 1);
    VT100.setBackgroundColor(TERMINAL_STATUS_LINE_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_STATUS_LINE_FOREGROUND_COLOR);
    this->stream->print(headerMessage);
    VT100.clearLineAfter();
    VT100.setCursor(TERMINAL_HEADER_LINE, TERMINAL_WIDTH + 1);
    VT100.setBackgroundColor(TERMINAL_BACKGROUND_COLOR);
    VT100.setTextColor(TERMINAL_FOREGROUND_COLOR);
    VT100.clearLineAfter();
}

void Terminal::showKeyFingerprint()
{
    this->showKeyFingerprintUntil = millis() + TEMRINAL_KEY_FINGERPRINT_DISPLAY_TIMEOUT;
}

void Terminal::showProgress(byte progressPercentile)
{
    static byte lastProgress = 0;

    if (lastProgress == progressPercentile)
    {
        return;
    }

    lastProgress = progressPercentile;

    char progressMessage[TERMINAL_WIDTH];

    sprintf(progressMessage, TXT_PROGRESS_PROTOTYPE, progressPercentile);

    VT100.setCursor(TERMINAL_FIRST_CANVAS_LINE + 5, (TERMINAL_WIDTH - strlen(progressMessage)) / 2);
    this->stream->print(progressMessage);

    this->resetInactivityTimer();
}

/**
 * See https://playground.arduino.cc/Code/AvailableMemory/
 **/
int Terminal::getFreeRamBytes()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

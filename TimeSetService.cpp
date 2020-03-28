#include "TimeSetService.h"

TimeSetService::TimeSetService(Terminal *terminal)
{
    this->terminal = terminal;
    this->rtc = uRTCLib(0x68);
}

bool TimeSetService::start()
{
    char buffer[TERMINAL_WIDTH];
    uint16_t year, month, day, hour, minute;

    this->terminal->readString(TXT_ENTER_DATE, buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 5, TERMINAL_LEFT_MENU_FIRST_COLUMN);

    if (sscanf(buffer, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute))
    {
        this->rtc.set(0, minute, hour, 0, day, month, year % 100);
    }

    return false;
}
#include "TimeSetService.h"

TimeSetService::TimeSetService(Terminal *terminal)
{
    this->terminal = terminal;    
}

bool TimeSetService::start()
{
    char buffer[TERMINAL_WIDTH];

    uRTCLib *rtc = new uRTCLib(0x68);

    uint16_t year, month, day, hour, minute;

    this->terminal->readString(TXT_ENTER_DATE, buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 5, TERMINAL_LEFT_MENU_FIRST_COLUMN);

    if (sscanf(buffer, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute))
    {
        rtc->set(0, minute, hour, 0, day, month, year % 100);
    }

    delete rtc;

    return false;
}

#include "PomodoroModeExecutor.h"

void PomodoroModeExecutor::doLoop()
{
    unsigned long pomodoroTime = (this->getTimeSinceModeChange() / 50) % 255;

    CRGB color = CRGB::DarkRed;
    CRGB color2 = pomodoroTime < 127 ? CRGB::White : CRGB::DarkRed;

    this->floodLight->setFade(255 - pomodoroTime, 255 - (pomodoroTime < 127 ? 0 : (pomodoroTime - 127)));
    this->floodLight->setColor(color, color2);
}

void PomodoroModeExecutor::doEnterMode()
{
}

void PomodoroModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);    
}

CRGB PomodoroModeExecutor::getModeSignatureColor()
{
    return CRGB::Orange;
}
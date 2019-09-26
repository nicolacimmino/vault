
#include "PomodoroModeExecutor.h"

void PomodoroModeExecutor::doLoop()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    unsigned long pomodoroTime = (this->getTimeSinceModeChange() / 50) % 255;

    CRGB color = CRGB::DarkRed;
    CRGB color2 = CRGB::White;

    this->floodLight->setFade(255 - pomodoroTime, 250);
    this->floodLight->setColor(color, color2);
}

CRGB PomodoroModeExecutor::getModeSignatureColor()
{
    return CRGB::Orange;
}

#include "NightlightModeExecutor.h"

void NighlightModeExecutor::doLoop()
{
    if (abs(this->lastX - this->accelerometer->getX()) > 64 || abs(this->lastY - this->accelerometer->getY()) > 64 || abs(this->lastZ - this->accelerometer->getZ()) > 64)
    {
        this->activeUntil = this->getTimeSinceModeChange() + 5000;

        this->floodLight->setColor(CRGB::NavajoWhite);
    }

    this->floodLight->setFade(this->fade);

    if (this->activeUntil < this->getTimeSinceModeChange())
    {
        this->floodLight->setColor(CRGB::Black);
    }

    this->lastX = this->accelerometer->getAveragedX();
    this->lastY = this->accelerometer->getAveragedY();
    this->lastZ = this->accelerometer->getAveragedZ();
}

void NighlightModeExecutor::doEnterMode()
{
    this->floodLight->setColor(CRGB::NavajoWhite);
    this->fade = 250;
}

void NighlightModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
    this->floodLight->setFade(255);
}

ColorsTuple NighlightModeExecutor::getModeSignatureColor()
{
    return {CRGB::NavajoWhite, CRGB::DarkSlateGray };
}

void NighlightModeExecutor::doOnShake()
{
    this->fade = this->fade == 250 ? 50 : 250;
}

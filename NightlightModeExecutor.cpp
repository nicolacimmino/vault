
#include "NightlightModeExecutor.h"

void NighlightModeExecutor::doLoop()
{
    if (abs(this->lastX - this->accelerometer->getX()) > 64 || abs(this->lastY - this->accelerometer->getY()) > 64 || abs(this->lastZ - this->accelerometer->getZ()) > 64)
    {
        this->activeUntil = this->getTimeSinceModeChange() + 5000;

        this->floodLight->setColor(this->lightColor);
    }

    this->floodLight->setFade(this->fade);

    if (!this->latched && this->activeUntil < this->getTimeSinceModeChange())
    {
        this->floodLight->setColor(CRGB::Black);
    }

    this->lastX = this->accelerometer->getAveragedX();
    this->lastY = this->accelerometer->getAveragedY();
    this->lastZ = this->accelerometer->getAveragedZ();
}

void NighlightModeExecutor::doEnterMode()
{
    this->floodLight->setColor(this->lightColor);
    this->fade = 250;
}

void NighlightModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
    this->floodLight->setFade(255);
}

ColorsTuple NighlightModeExecutor::getModeSignatureColor()
{
    return {this->lightColor, CRGB::DarkSlateGray};
}

void NighlightModeExecutor::doOnShake()
{
    if(this->latched) {
        return;
    }
    
    this->fade = this->fade == 250 ? 50 : 250;
}

void NighlightModeExecutor::doOnClick()
{
    if (abs(this->accelerometer->getX()) < 96)
    {
        return;
    }

    if (this->accelerometer->getX() > 0)
    {
        this->latched = !this->latched;
        this->ledBarController->showBar(this->latched ? 1 : 0);
    }
}

#include "RainbowModeExecutor.h"

void RainbowModeExecutor::doLoop()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    this->floodLight->setColor(CRGB::CRGB(this->accelerometer->getX() + 127, this->accelerometer->getY() + 127, this->accelerometer->getZ() + 127));
    this->floodLight->setFade(255);
}

CRGB RainbowModeExecutor::getModeSignatureColor()
{
    return CRGB::Blue;
}

#include "RainbowModeExecutor.h"

void RainbowModeExecutor::doLoop()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    this->floodLight->setFade(0);
    this->floodLight->setColor(CRGB::CRGB(this->getColorComponent(this->accelerometer->getAveragedX()), this->getColorComponent(this->accelerometer->getAveragedY()), this->getColorComponent(this->accelerometer->getAveragedZ())));
    
}

uint8_t RainbowModeExecutor::getColorComponent(int8_t axisReading)
{
    return abs(axisReading) * 2;
}

CRGB RainbowModeExecutor::getModeSignatureColor()
{
    return CRGB::Blue;
}
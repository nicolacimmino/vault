
#include "RainbowModeExecutor.h"

void RainbowModeExecutor::doLoop()
{        
    this->floodLight->setColor(CRGB::CRGB(this->getColorComponent(this->accelerometer->getAveragedX()), this->getColorComponent(this->accelerometer->getAveragedY()), this->getColorComponent(this->accelerometer->getAveragedZ())));
    
}

void RainbowModeExecutor::doEnterMode()
{
    this->floodLight->setFade(0);
}

void RainbowModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
    this->floodLight->setFade(255);
}

uint8_t RainbowModeExecutor::getColorComponent(int8_t axisReading)
{
    return abs(axisReading) * 2;
}

CRGB RainbowModeExecutor::getModeSignatureColor()
{
    return CRGB::Blue;
}
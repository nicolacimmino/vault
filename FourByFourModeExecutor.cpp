
#include "FourByFourModeExecutor.h"

void FourByFourModeExecutor::doLoop()
{
    uint32_t timePhase = (millis() / 1000) % 5;

    this->ledBarController->setDim();
    if (this->accelerometer->getAveragedY() < -90)
    {
        this->ledBarController->setFullBrightness();
    }

    this->ledBarController->showBar(timePhase);

    float fade = 255 - (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;

    this->floodLight->setColor(CRGB::DarkGreen);
    this->floodLight->setFade(fade);
}

void FourByFourModeExecutor::doEnterMode()
{
    
}

void FourByFourModeExecutor::doExitMode()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    this->floodLight->setColor(CRGB::Black);
}

CRGB FourByFourModeExecutor::getModeSignatureColor()
{
    return CRGB::DarkGreen;
}
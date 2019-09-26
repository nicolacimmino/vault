#include "ModeExecutor.h"

void ModeExecutor::begin(FloodLight *floodLight, LEDBarController *ledBarController, Accelerometer *accelerometer)
{
    this->floodLight = floodLight;
    this->ledBarController = ledBarController;
    this->accelerometer = accelerometer;
}

void ModeExecutor::enterMode()
{
    this->modeChangeTime = millis();
    this->modeActive = true;

    this->doEnterMode();
}

void ModeExecutor::exitMode()
{
    this->doExitMode();

    this->modeActive = false;
}

void ModeExecutor::loop()
{
    if (!this->modeActive)
    {
        return;
    }

    if (this->getTimeSinceModeChange() < 1000)
    {
        this->floodLight->setFade(0);
        this->floodLight->setColor(CRGB::Red, this->getModeSignatureColor());

        return;
    }

    doLoop();
}

uint32_t ModeExecutor::getTimeSinceModeChange()
{
    return millis() - this->modeChangeTime;
}
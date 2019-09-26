
#include "SquareBreathModeExecutor.h"

void SquareBreathModeExecutor::doLoop()
{
    this->updateElapsedTimeBar();
    this->breatheFloodLight();
}

void SquareBreathModeExecutor::breatheFloodLight()
{
    unsigned long cycleTime = this->getTimeSinceModeChange() % (this->sideDuration * 4);

    uint8_t fade = 0;

    if (cycleTime < this->sideDuration)
    {
        fade = (255.0 * (this->sideDuration - cycleTime)) / this->sideDuration;
    }
    else if (cycleTime < this->sideDuration * 2.0)
    {
        fade = 0;
    }
    else if (cycleTime < this->sideDuration * 3)
    {
        fade = 255.0 * ((cycleTime - (this->sideDuration * 2.0)) / this->sideDuration);
    }
    else
    {
        fade = 255;
    }

    fade = min(250, fade);

    this->floodLight->setColor((cycleTime % this->sideDuration < 300) ? CRGB::DarkOrange : CRGB::Green);
    this->floodLight->setFade(fade);
}

void SquareBreathModeExecutor::updateElapsedTimeBar()
{
    uint8_t elapsedFiveMinutesBlocks = floor(this->getTimeSinceModeChange() / (5 * 60 * 1000));

    this->ledBarController->setDim();
    if (this->accelerometer->getAveragedY() < -90)
    {
        this->ledBarController->setFullBrightness();
    }

    this->ledBarController->showBar(elapsedFiveMinutesBlocks);
}

void SquareBreathModeExecutor::doEnterMode()
{
}

void SquareBreathModeExecutor::doExitMode()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    this->floodLight->setColor(CRGB::Black);
}

CRGB SquareBreathModeExecutor::getModeSignatureColor()
{
    return CRGB::DarkGreen;
}
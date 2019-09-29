
#include "PomodoroModeExecutor.h"

void PomodoroModeExecutor::doLoop()
{
    if (this->getPomodoroTimeSeconds() > this->pomodoroDurationSeconds)
    {
        this->breakPattern();
        return;
    }

    this->timerPattern();
}

void PomodoroModeExecutor::breakPattern()
{
    // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
    // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
    float val = (exp(sin(millis() / 3000.0 * PI)) - 0.36787944) * 108.0;
    this->floodLight->setColor(CRGB::DarkRed, CRGB::Green);
    this->floodLight->setFade(val, 255 - val);
}

void PomodoroModeExecutor::timerPattern()
{
    uint8_t val = 255 * (this->getPomodoroTimeSeconds() / (float)this->pomodoroDurationSeconds);
    this->floodLight->setColor(CRGB::DarkRed, CRGB::White);
    this->floodLight->setFade(255 - val, max(val, 127));
}

uint16_t PomodoroModeExecutor::getPomodoroTimeSeconds()
{
    return (this->getTimeSinceModeChange() / 1000);
}

void PomodoroModeExecutor::doEnterMode()
{
}

void PomodoroModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
}

ColorsTuple PomodoroModeExecutor::getModeSignatureColor()
{
    return {CRGB::Orange, CRGB::Yellow};
}

void PomodoroMode::doOnShake()
{
    
}
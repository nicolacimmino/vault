#include "BrightnessChanger.h"

void BrightnessChanger::begin(FloodLight *floodLight, MemoryController *memoryController)
{
    this->memoryController = memoryController;

    if (this->memoryController->getByte(MEMORY_GLOBAL_BRIGHTNESS_CAP) > 0)
    {
        this->brightness = this->memoryController->getByte(MEMORY_GLOBAL_BRIGHTNESS_CAP);
    }

    this->floodLight = floodLight;
    this->activeUntil = millis() + this->timeout;
    this->floodLight->setColor(CRGB::Yellow);
    this->floodLight->setBrightness(this->brightness);
    this->floodLight->loop();
}

void BrightnessChanger::loop()
{
    if (this->activeUntil == 0)
    {
        return;
    }

    if (this->activeUntil < millis())
    {
        this->floodLight->setBrightness(max(this->memoryController->getByte(MEMORY_GLOBAL_BRIGHTNESS_CAP), 50));

        this->activeUntil = 0;
    }
}

void BrightnessChanger::onClick()
{
    this->brightness = max(10, (this->brightness + 50) % 250);
    this->floodLight->setBrightness(this->brightness);
    this->activeUntil = millis() + this->timeout;
}

void BrightnessChanger::onLongPress()
{
    this->floodLight->override(200, CRGB::Black, 0);
    this->activeUntil = 0;
    this->memoryController->setByte(MEMORY_GLOBAL_BRIGHTNESS_CAP, this->brightness);
}

bool BrightnessChanger::isActive()
{
    return this->activeUntil != 0;
}

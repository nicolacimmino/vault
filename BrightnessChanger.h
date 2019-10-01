#ifndef __BRIGHTNESSCHANGER_H__
#define __BRIGHTNESSCHANGER_H__

#include <Arduino.h>
#include "FloodLight.h"
#include "MemoryController.h"

class BrightnessChanger
{
public:
    void begin(FloodLight *floodLight, MemoryController *memoryController);
    void loop();
    void onClick();
    void onLongPress();
    bool isActive();

private:
    FloodLight *floodLight;
    uint8_t brightness = 50;
    uint32_t activeUntil = 0;
    uint16_t timeout = 4000;
    MemoryController *memoryController;
};

#endif
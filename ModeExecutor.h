#ifndef __MODEEXECUTOR_H__
#define __MODEEXECUTOR_H__

#include <Arduino.h>
#include "FloodLight.h"
#include "LEDBarController.h"
#include "Accelerometer.h"

class ModeExecutor
{
public:
    void begin(FloodLight *floodLight, LEDBarController *ledBarController, Accelerometer *accelerometer);
    void loop();
    void enterMode();
    void exitMode();

protected:
    virtual void doLoop();
    virtual void doEnterMode();
    virtual void doExitMode();
    virtual CRGB getModeSignatureColor();
    uint32_t getTimeSinceModeChange();
    FloodLight *floodLight;
    LEDBarController *ledBarController;
    Accelerometer *accelerometer;

private:
    uint32_t modeChangeTime;
    bool modeActive = false;

};

#endif
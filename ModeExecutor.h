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
    void onTiltX(bool positive);

protected:
    virtual void doLoop();
    virtual void doEnterMode();
    virtual void doExitMode();
    virtual void doOnTiltX(bool positive);
    virtual CRGB getModeSignatureColor();
    uint32_t getTimeSinceModeChange();
    void inhibitLoopFor(uint32_t milliseconds);
    void updateElapsedTimeBar(uint32_t dotDurationmS);
    FloodLight *floodLight;
    LEDBarController *ledBarController;
    Accelerometer *accelerometer;
    bool exerciseEnded = false;
    
private:
    uint32_t modeChangeTime;
    bool modeActive = false;
    uint32_t inhibitLoopUntil = 0;
};

#endif
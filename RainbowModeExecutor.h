#ifndef __RAINBOWMODEEXECUTOR_H__
#define __RAINBOWMODEEXECUTOR_H__

#include "ModeExecutor.h"

class RainbowModeExecutor : public ModeExecutor
{

public:
protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    CRGB getModeSignatureColor();
private:
    uint8_t getColorComponent(int8_t axisReading);

};

#endif
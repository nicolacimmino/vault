#ifndef __NIGHTLIGHTMODEEXECUTOR_H__
#define __NIGHTLIGHTMODEEXECUTOR_H__

#include "ModeExecutor.h"

class NighlightModeExecutor : public ModeExecutor
{

public:
protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    CRGB getModeSignatureColor();
    void doOnShake();

private:
    int8_t lastX;
    int8_t lastY;
    int8_t lastZ;
    uint32_t activeUntil = 0;
    uint8_t fade = 250;
};

#endif
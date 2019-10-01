#ifndef __NIGHTLIGHTMODEEXECUTOR_H__
#define __NIGHTLIGHTMODEEXECUTOR_H__

#include "ModeExecutor.h"

class NighlightModeExecutor : public ModeExecutor
{

public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnShake();
    void doOnClick();

private:
    int8_t lastX;
    int8_t lastY;
    int8_t lastZ;
    uint32_t activeUntil = 0;
    uint8_t fade = 250;
    bool latched = false;
    CRGB lightColor = CRGB::CRGB(CHSV::CHSV(42, 200 , 255));
};

#endif
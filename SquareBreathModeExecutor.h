#ifndef __SQAREBREATHMODEEXECUTOR_H__
#define __SQAREBREATHMODEEXECUTOR_H__

#include "ModeExecutor.h"

class SquareBreathModeExecutor : public ModeExecutor
{

public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnTiltX(bool positive);

private:
    void breatheFloodLight();
    unsigned long getEffectiveSideDuration();
    uint32_t sideDuration = 4000;
};

#endif
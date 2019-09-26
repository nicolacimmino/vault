#ifndef __SQAREBREATHMODEEXECUTOR_H__
#define __SQAREBREATHMODEEXECUTOR_H__

#include "ModeExecutor.h"

class SquareBreathModeExecutor : public ModeExecutor
{

public:
    void onTiltX(bool positive);

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    CRGB getModeSignatureColor();

private:
    void updateElapsedTimeBar();
    void breatheFloodLight();
    unsigned long getEffectiveSideDuration();
    uint32_t sideDuration = 4000;
    bool exerciseEnded = false;
    
};

#endif
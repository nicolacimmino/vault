#ifndef __SQAREBREATHMODEEXECUTOR_H__
#define __SQAREBREATHMODEEXECUTOR_H__

#include "ModeExecutor.h"

class SquareBreathModeExecutor : public ModeExecutor
{

public:
protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    CRGB getModeSignatureColor();
private:
    void updateElapsedTimeBar();
    void breatheFloodLight();
    uint32_t sideDuration = 4000;

};

#endif
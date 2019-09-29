#ifndef __POMODOROMODEEXECUTOR_H__
#define __POMODOROMODEEXECUTOR_H__

#include "ModeExecutor.h"

class PomodoroModeExecutor : public ModeExecutor
{

public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnShake();
    
private:
    void breakPattern();
    void timerPattern();
    uint16_t pomodoroDurationSeconds = 30;
    uint16_t getPomodoroTimeSeconds();
};

#endif
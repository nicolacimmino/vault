#ifndef __POMODOROMODEEXECUTOR_H__
#define __POMODOROMODEEXECUTOR_H__

#include "ModeExecutor.h"

class PomodoroModeExecutor : public ModeExecutor
{

public:
protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    CRGB getModeSignatureColor();
private:

};

#endif
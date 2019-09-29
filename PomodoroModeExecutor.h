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

private:
};

#endif
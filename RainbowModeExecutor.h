#ifndef __RAINBOWMODEEXECUTOR_H__
#define __RAINBOWMODEEXECUTOR_H__

#include "ModeExecutor.h"

class RainbowModeExecutor : public ModeExecutor
{

public:
protected:
    void doLoop();
    CRGB getModeSignatureColor();
private:


};

#endif
#ifndef __ALTERNATENOSTRILBREATHMODEEXECUTOR_H__
#define __ALTERNATENOSTRILBREATHMODEEXECUTOR_H__

#include "ModeExecutor.h"

class AlternateNostrilBreathModeExecutor : public ModeExecutor
{

public:
    
protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    struct ModeExecutor::ColorsTuple getModeSignatureColor();

private:    
    void breatheFloodLight();        
    bool leftNostril = true;
};

#endif
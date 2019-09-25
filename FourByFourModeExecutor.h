#ifndef __FOURBYFOURMODEEXECUTOR_H__
#define __FOURBYFOURMODEEXECUTOR_H__

#include "ModeExecutor.h"

class FourByFourModeExecutor : public ModeExecutor
{

public:
protected:
    void doLoop();
    CRGB getModeSignatureColor();
private:


};

#endif
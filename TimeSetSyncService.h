#ifndef __VAULT_TIMESET_SYNC_SERVICE_H__
#define __VAULT_TIMESET_SYNC_SERVICE_H__

#include <Functor.h>
#include <Arduino.h>
#include "SafeBuffer.h"
#include <uRTCLib.h>
#include "Terminal.h"

class TimeSetSyncService
{
private:
    Terminal *terminal;
    uRTCLib rtc;

protected:
public:
    TimeSetSyncService(Terminal *terminal);
    void execute();
};

#endif
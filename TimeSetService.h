#ifndef __VAULT_TIMESET_SYNC_SERVICE_H__
#define __VAULT_TIMESET_SYNC_SERVICE_H__

#include <Functor.h>
#include <Arduino.h>
#include <uRTCLib.h>
#include "Terminal.h"

class TimeSetService
{
private:
    Terminal *terminal;

protected:
public:
    TimeSetService(Terminal *terminal);
    bool start();
};

#endif
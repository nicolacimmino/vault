#ifndef __VAULT_FULL_WIPE_SERVICE_H__
#define __VAULT_FULL_WIPE_SERVICE_H__

#include "hardware.h"
#include "Service.h"
#include "Terminal.h"
#include "messages.h"
#include <EEPROM.h>

#define WIPE_ADDRESSES_PER_LOOP 0xFF

class FullWipeService : public Service
{
private:
    uint16_t address;
    Terminal *terminal;

public:
    FullWipeService(Terminal *terminal, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start();
    void loop();
};

#endif
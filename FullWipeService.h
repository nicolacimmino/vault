#ifndef __VAULT_FULL_WIPE_SERVICE_H__
#define __VAULT_FULL_WIPE_SERVICE_H__

#include "Service.h"
#include "Storage.h"
#include "Terminal.h"
#include "messages.h"

#define WIPE_ADDRESSES_PER_LOOP 0xFF

class FullWipeService : public Service
{
private:
    uint16_t address;
    Storage *storage;
    Terminal *terminal;

public:
    FullWipeService(Terminal *terminal, Storage *storage, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);    
    bool start();
    void loop();
};

#endif
#ifndef __VAULT_FULL_WIPE_SERVICE_H__
#define __VAULT_FULL_WIPE_SERVICE_H__

#include "Service.h"
#include "Storage.h"

#define WIPE_ADDRESSES_PER_LOOP 0xFF

class FullWipeService : public Service
{
private:
    uint16_t address;
    Storage storage;

public:
    FullWipeService(const Functor1<byte> &reportProgress, const Functor1<byte> &reportCompletion);    
    void start();
    void loop();
};

#endif
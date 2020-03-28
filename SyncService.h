#ifndef __VAULT_SYNC_SERVICE_H__
#define __VAULT_SYNC_SERVICE_H__

#include <Functor.h>
#include <Arduino.h>

class SyncService
{
private:
protected:
public:
    virtual void execute() = 0;
};

#endif
#ifndef __VAULT_ADD_PASSWORD_SERVICE_H__
#define __VAULT_ADD_PASSWORD_SERVICE_H__

#include "Service.h"
#include "Terminal.h"
#include <Keyboard.h>
#include "hardware.h"
#include <EEPROM.h>
#include "EncryptedStore.h"

class AddPasswordService : public Service
{
private:
    Terminal *terminal;
    EncryptedStore *encryptedStore;

public:
    AddPasswordService(Terminal *terminal, EncryptedStore *encryptedStore, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start(byte arg = 0);
    bool loop();
};

#endif
#ifndef __VAULT_RETRIEVE_PASSWORD_SERVICE_H__
#define __VAULT_RETRIEVE_PASSWORD_SERVICE_H__

#include "Service.h"
#include "Terminal.h"
#include <Keyboard.h>
#include "hardware.h"
#include <EEPROM.h>
#include "EncryptedStore.h"

#define BACKUP_LINE_SIZE 64
#define RETRIEVE_PASSWORD_ACTION_FULL 0
#define RETRIEVE_PASSWORD_ACTION_PARTIAL 1

class RetrievePasswordService : public Service
{
private:
    Terminal *terminal;
    EncryptedStore *encryptedStore;
    byte selectedPasswordIndex;

public:
    RetrievePasswordService(Terminal *terminal, EncryptedStore *encryptedStore, byte selectedPasswordIndex, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start(byte arg = 0);
    bool loop();
};

#endif
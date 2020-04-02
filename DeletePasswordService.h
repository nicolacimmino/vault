#ifndef __VAULT_DELETE_PASSWORD_SERVICE_H__
#define __VAULT_DELETE_PASSWORD_SERVICE_H__

#include "Service.h"
#include "Terminal.h"
#include <Keyboard.h>
#include "hardware.h"
#include <EEPROM.h>
#include "EncryptedStore.h"

#define BACKUP_LINE_SIZE 64

class DeletePasswordService : public Service
{
private:
    Terminal *terminal;
    EncryptedStore *encryptedStore;
    byte selectedPasswordIndex;

public:
    DeletePasswordService(Terminal *terminal, EncryptedStore *encryptedStore, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start(byte arg = 0);
    bool loop();
};

#endif
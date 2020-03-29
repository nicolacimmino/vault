#ifndef __VAULT_BACKUP_SERVICE_H__
#define __VAULT_BACKUP_SERVICE_H__

#include "Service.h"
#include "Storage.h"
#include "Terminal.h"
#include <Keyboard.h>
#include "hardware.h"

#define BAKCUP_ADDRESSES_PER_LINE 16
#define BACKUP_LINES_PER_LOOP 16

class BackupService : public Service
{
private:
    char *asciiPrint;
    char *addressBuffer;
    uint16_t backupAddress;
    Storage *storage;
    Terminal *terminal;
    bool backupStarted = false;

public:
    BackupService(Terminal *terminal, Storage *storage, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    ~BackupService();
    bool start();
    void loop();
};

#endif
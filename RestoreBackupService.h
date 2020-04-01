#ifndef __VAULT_RESTORE_BACKUP_SERVICE_H__
#define __VAULT_RESTORE_BACKUP_SERVICE_H__

#include "Service.h"
#include "Terminal.h"
#include <Keyboard.h>
#include "hardware.h"
#include <EEPROM.h>

#define BACKUP_LINE_SIZE 64

class RestoreBackupService : public Service
{
private:
    uint16_t backupRestoreAddress;    
    Terminal *terminal;
    bool backupRestoreStarted = false;

public:
    RestoreBackupService(Terminal *terminal, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start();
    void loop();
};

#endif
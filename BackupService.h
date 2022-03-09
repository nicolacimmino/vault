#ifndef __VAULT_BACKUP_SERVICE_H__
#define __VAULT_BACKUP_SERVICE_H__

#include "Service.h"
#include "Terminal.h"
#include <Keyboard.h>
#include "hardware.h"
#include <EEPROM.h>
#ifndef HW_BUTTON
#include <ADCTouch.h>
#endif

#define BAKCUP_ADDRESSES_PER_LINE 16
#define BACKUP_LINES_PER_LOOP 4

class BackupService : public Service
{
private:
    uint16_t backupAddress;    
    Terminal *terminal;
    bool backupStarted = false;
    int touchRef;

public:
    BackupService(Terminal *terminal, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start(byte arg = 0);
    bool loop();
};

#endif
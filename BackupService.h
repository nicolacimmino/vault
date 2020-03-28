#ifndef __VAULT_BACKUP_SERVICE_H__
#define __VAULT_BACKUP_SERVICE_H__

#include "Service.h"
#include "SafeBuffer.h"
#include "Storage.h"
#include <Keyboard.h>

#define BAKCUP_ADDRESSES_PER_LINE 16
#define BACKUP_LINES_PER_LOOP 16

class BackupService : public Service
{
private:
    SafeBuffer *asciiPrint;
    SafeBuffer *addressBuffer;
    uint16_t backupAddress;
    Storage storage;

public:
    BackupService(const Functor1<byte> &reportProgress, const Functor1<byte> &reportCompletion);
    ~BackupService();
    bool start();
    void loop();
};

#endif
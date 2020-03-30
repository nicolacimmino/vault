#include "RestoreBackupService.h"

RestoreBackupService::RestoreBackupService(Terminal *terminal, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->terminal = terminal;
}

bool RestoreBackupService::start()
{
    if (!this->terminal->askYesNoQuestion(TXT_BACKUP_RESTORE_CONFIRMATION, true))
    {
        return false;
    }

    this->running = true;
    this->backupRestoreAddress = STORAGE_BASE;

    this->terminal->alert(TXT_BACKUP_RESTORE_IN_PROGRESS);

    return true;
}

void RestoreBackupService::loop()
{
    while (!Serial.available())
    {
        delay(1);
    }

    char *line = new char[128];
    
    Serial.readBytesUntil('\n', line, 127);

    this->backupRestoreAddress = strtol(strtok(line, ' '), NULL, 16);
    byte offset = 0;

    while (byte value = strtol(strtok(NULL, '.'), NULL, 16))
    {
        EEPROM.write(this->backupRestoreAddress + offset, value);
        offset++;
    }

    delete line;

    this->reportProgress(((unsigned long)(this->backupRestoreAddress - STORAGE_BASE) * 100) / STORAGE_SIZE);

    if (this->backupRestoreAddress >= (STORAGE_BASE + STORAGE_SIZE))
    {
        this->running = false;
        this->reportCompletion();
    }
}
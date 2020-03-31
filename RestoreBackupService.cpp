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
    char *line = new char[64];
    memset(line, 0, 64);

    Serial.write(FLOW_CONTROL_XON);
    Serial.flush();

    while (!Serial.available())
    {
        delay(1);
    }

    Serial.readBytesUntil('\r', line, 64);

    Serial.write(FLOW_CONTROL_XOFF);
    Serial.flush();

    this->backupRestoreAddress = strtol(strtok(line, " ."), NULL, 16);

    while (char *token = strtok(NULL, " ."))
    {
        byte value = strtol(token, NULL, 16);

        EEPROM.write(this->backupRestoreAddress, value);
        this->backupRestoreAddress++;
    }

    this->reportProgress(((unsigned long)(this->backupRestoreAddress - STORAGE_BASE) * 100) / STORAGE_SIZE);

    if (this->backupRestoreAddress >= (STORAGE_BASE + STORAGE_SIZE))
    {
        this->running = false;
        this->reportCompletion();
    }

    delete line;
}
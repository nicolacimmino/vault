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
    char *line = new char[BACKUP_LINE_SIZE];
    
    this->terminal->flowControl(true);
    this->terminal->readLine(line, BACKUP_LINE_SIZE);    
    this->terminal->flowControl(false);

    this->backupRestoreAddress = strtol(strtok(line, " ."), NULL, 16);

    while (char *token = strtok(NULL, " ."))
    {
        EEPROM.write(this->backupRestoreAddress++, strtol(token, NULL, 16));        
    }

    this->reportProgress(((unsigned long)(this->backupRestoreAddress - STORAGE_BASE) * 100) / STORAGE_SIZE);

    if (this->backupRestoreAddress >= (STORAGE_BASE + STORAGE_SIZE))
    {
        this->running = false;
        this->reportCompletion();
    }

    delete line;
}
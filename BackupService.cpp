#include "BackupService.h"

BackupService::BackupService(Terminal *terminal, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->terminal = terminal;    
}

bool BackupService::start(byte arg = 0)
{
    if (!this->terminal->askYesNoQuestion(TXT_BACKUP_FULL_CONFIRMATION))
    {
        this->reportCompletion();

        return false;
    }

    this->terminal->alert(TXT_TOUCH_TO_START_BACKUP);

    this->running = true;
    this->backupAddress = STORAGE_BASE;
#ifndef HW_BUTTON
    this->touchRef = ADCTouch.read(A1, 500);
#endif
    return true;
}

bool BackupService::loop()
{
    if(!this->running) {
        return false;
    }

#ifndef HW_BUTTON
bool buttonPressed = ADCTouch.read(A1) - this->touchRef < TOUCH_THRESHOLD;
#else
bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
#endif

    if (!this->backupStarted && !buttonPressed)
    {
        return true;
    }

    if (!this->backupStarted)
    {
        this->terminal->alert(TXT_BACKUP_IN_PROGRESS);
        this->backupStarted = true;
    }

    this->reportProgress(((unsigned long)(this->backupAddress - STORAGE_BASE) * 100) / STORAGE_SIZE);

    for (uint16_t addressOffset = 0; addressOffset < BAKCUP_ADDRESSES_PER_LINE * BACKUP_LINES_PER_LOOP; addressOffset++)
    {
        uint16_t actualBackupAddress = this->backupAddress + addressOffset;

        if (actualBackupAddress >= (STORAGE_BASE + STORAGE_SIZE))
        {
            break;
        }

        byte value = EEPROM.read(actualBackupAddress);

        if (addressOffset % BAKCUP_ADDRESSES_PER_LINE == 0)
        {
            for (int8_t ix = 4; ix >= 0; ix--)
            {
                Keyboard.print((actualBackupAddress >> (ix * 4)) & 0xF, HEX);
            }
            Keyboard.print(" ");
        }

        Keyboard.print(value >> 4, HEX);
        Keyboard.print(value & 0xF, HEX);

        Keyboard.print((addressOffset % BAKCUP_ADDRESSES_PER_LINE == BAKCUP_ADDRESSES_PER_LINE - 1) ? "\n" : ".");
    }

    this->backupAddress += (BAKCUP_ADDRESSES_PER_LINE * BACKUP_LINES_PER_LOOP);

    if (this->backupAddress >= (STORAGE_BASE + STORAGE_SIZE))
    {
        this->running = false;
        this->reportCompletion();

        return false;
    }

    return true;
}
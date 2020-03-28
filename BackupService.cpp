#include "BackupService.h"

BackupService::BackupService(Terminal *terminal, Storage *storage, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->terminal = terminal;
    this->storage = storage;
    this->asciiPrint = new SafeBuffer(16);
    this->addressBuffer = new SafeBuffer(5);
}

BackupService::~BackupService()
{
    delete this->addressBuffer;
    delete this->asciiPrint;
}

bool BackupService::start()
{
    if (!this->terminal->askYesNoQuestion(TXT_BACKUP_FULL_CONFIRMATION))
    {
        return false;
    }

    this->terminal->alert("Ready. Press button to type.");

    this->running = true;
    this->backupAddress = STORAGE_BASE;

    return true;
}

void BackupService::loop()
{
    if (!this->backupStarted && digitalRead(BUTTON_SENSE) == HIGH)
    {
        return;
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

        byte value = this->storage->read(actualBackupAddress);

        if (addressOffset % BAKCUP_ADDRESSES_PER_LINE == 0)
        {
            sprintf(addressBuffer->getBuffer(), "%04X ", actualBackupAddress);
            Keyboard.print(addressBuffer->getBuffer());
        }

        Keyboard.print(value >> 4, HEX);
        Keyboard.print(value & 0xF, HEX);
        asciiPrint->setChar(addressOffset % BAKCUP_ADDRESSES_PER_LINE, value > 31 && value < 127 ? (char)value : '.');

        if (addressOffset % BAKCUP_ADDRESSES_PER_LINE == 15)
        {
            Keyboard.print("\t");
            Keyboard.print(asciiPrint->getBuffer());
            Keyboard.print("\n");
            continue;
        }

        Keyboard.print(".");
    }

    this->backupAddress += (BAKCUP_ADDRESSES_PER_LINE * BACKUP_LINES_PER_LOOP);

    if (this->backupAddress >= (STORAGE_BASE + STORAGE_SIZE))
    {
        this->running = false;
        this->reportCompletion();
    }
}
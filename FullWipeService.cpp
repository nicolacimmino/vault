#include "FullWipeService.h"

FullWipeService::FullWipeService(Terminal *terminal, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->terminal = terminal;    
    this->address = STORAGE_BASE;
}

bool FullWipeService::start()
{    
    if (!this->terminal->askYesNoQuestion(TXT_WIPE_FULL_CONFIRMATION, true))
    {
        return false;
    }

    this->terminal->alert(TXT_WIPING);

    this->running = true;

    return true;
}

void FullWipeService::loop()
{
    this->reportProgress(((unsigned long)(this->address - STORAGE_BASE) * 100) / STORAGE_SIZE);

    for (uint16_t addressOffset = 0; addressOffset < WIPE_ADDRESSES_PER_LOOP; addressOffset++)
    {
        EEPROM.write(this->address + addressOffset, 0);
    }

    this->address += WIPE_ADDRESSES_PER_LOOP;

    if (this->address >= (STORAGE_BASE + STORAGE_SIZE))
    {
        this->running = false;
        this->reportCompletion();
    }
}
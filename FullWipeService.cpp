#include "FullWipeService.h"

FullWipeService::FullWipeService(const Functor1<byte> &reportProgress, const Functor1<byte> &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->address = STORAGE_BASE;
}

void FullWipeService::start()
{
    this->running = true;
}

void FullWipeService::loop()
{
    this->reportProgress((byte)floor(100.0 * (((float)this->address - STORAGE_BASE) / STORAGE_SIZE)));

    for (uint16_t addressOffset = 0; addressOffset < WIPE_ADDRESSES_PER_LOOP; addressOffset++)
    {
        this->storage.write(this->address + addressOffset, 0);
    }

    this->address += WIPE_ADDRESSES_PER_LOOP;

    if (this->address >= (STORAGE_BASE + STORAGE_SIZE))
    {
        this->running = false;
        this->reportCompletion(0);
    }
}
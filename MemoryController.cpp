
#include "MemoryController.h"

uint8_t MemoryController::getByte(uint8_t address)
{
    return EEPROM.read(address - MEMORY_EEPROM_LO_BOUNDARY);
}

void MemoryController::setByte(uint8_t address, uint8_t value)
{
    if (address < MEMORY_EEPROM_LO_BOUNDARY || address > MEMORY_EEPROM_HI_BOUNDARY)
    {
        // For the time being we implement only the EEPROM.
        return;
    }

    return EEPROM.write(address - MEMORY_EEPROM_LO_BOUNDARY, value);
}
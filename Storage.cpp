#include "Storage.h"

Storage::Storage()
{
    this->eeprom = new uEEPROMLib(0x57);
}

Storage::~Storage()
{
    delete this->eeprom;
}

byte Storage::read(uint16_t address)
{
    if (this->isInternalEEPROMAddress(address))
    {
        return EEPROM.read(address - STORAGE_INTERNAL_EEPROM_BASE);
    }

    if (this->isExternalEEPROMAddress(address))
    {
        return this->eeprom->eeprom_read(address - STORAGE_EXTERNAL_EEPROM_BASE);
    }

    return 0xFF;
}

void Storage::write(uint16_t address, byte value)
{
    if (this->isInternalEEPROMAddress(address))
    {
        EEPROM.write(address - STORAGE_INTERNAL_EEPROM_BASE, value);
    }

    if (this->isExternalEEPROMAddress(address))
    {
        this->eeprom->eeprom_write(address - STORAGE_EXTERNAL_EEPROM_BASE, value);
    }
}

bool Storage::isInternalEEPROMAddress(uint16_t address)
{
    return address >= STORAGE_INTERNAL_EEPROM_BASE && address < STORAGE_INTERNAL_EEPROM_BASE + STORAGE_INTERNAL_EEPROM_SIZE;
}

bool Storage::isExternalEEPROMAddress(uint16_t address)
{
    return address >= STORAGE_EXTERNAL_EEPROM_BASE && address < STORAGE_EXTERNAL_EEPROM_BASE + STORAGE_EXTERNAL_EEPROM_SIZE;
}
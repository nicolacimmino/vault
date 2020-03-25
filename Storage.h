#ifndef __STORAGE_H__
#define __STORAGE_H__

#define STORAGE_INTERNAL_EEPROM_BASE 0
#define STORAGE_INTERNAL_EEPROM_SIZE 1024
#define STORAGE_EXTERNAL_EEPROM_BASE 0x400
#define STORAGE_EXTERNAL_EEPROM_SIZE 32768
#define STORAGE_BASE 0
#define STORAGE_SIZE (STORAGE_INTERNAL_EEPROM_SIZE + STORAGE_EXTERNAL_EEPROM_SIZE)

#include "Arduino.h"
#include <uEEPROMLib.h>
#include "EEPROM.h"

class Storage
{
private:
    uEEPROMLib eeprom;
    bool isInternalEEPROMAddress(uint16_t address);
    bool isExternalEEPROMAddress(uint16_t address);

public:
    Storage();
    byte read(uint16_t address);
    void write(uint16_t address, byte value);
};

#endif
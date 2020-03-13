#ifndef __PGEN__ENCRYPTED_STORE_H__
#define __PGEN__ENCRYPTED_STORE_H__

#include "Arduino.h"
#include "aes256_ctr.h"
#include "sha256.h"
#include "NoiseSource.h"
#include <EEPROM.h>
#include "SafeBuffer.h"

#define ENCRYPTED_STORE_EEPROM_SIZE 1024
#define ENCRYPTED_STORE_KEY_SIZE 32
#define ENCRYPTED_STORE_IV_SIZE 16
#define ENCRYPTED_STORE_BLOCK_SIZE 16
#define ENCRYPTED_STORE_DATA_SIZE 64
#define ENCRYPTED_STORE_LABEL_SIZE 16
#define ENCRYPTED_STORE_MAX_ENTRIES ENCRYPTED_STORE_EEPROM_SIZE / (ENCRYPTED_STORE_IV_SIZE + ENCRYPTED_STORE_DATA_SIZE + ENCRYPTED_STORE_LABEL_SIZE)
#define ENCRYPTED_STORE_FULL 255

class EncryptedStore
{
private:
    struct EncryptedEntry
    {
        byte iv[ENCRYPTED_STORE_IV_SIZE];
        byte cipher[ENCRYPTED_STORE_DATA_SIZE];
        char label[ENCRYPTED_STORE_LABEL_SIZE];
    };

    byte key[ENCRYPTED_STORE_KEY_SIZE];
    void generateIV(byte *iv);
    uint16_t getEncryptedEntryAddress(byte index);
    void safeStringCopy(char *destination, char *source, byte destinationSize);
    bool locked = true;

public:
    void unlock(SafeBuffer *masterPassword);
    void lock();
    void get(byte index, SafeBuffer *plainText);
    void getTokens(byte index, char *tokensList, SafeBuffer *plainText);
    void getLabel(byte index, char *label);
    void set(byte index, SafeBuffer *plainText, SafeBuffer *label);
    void wipe(byte index);
    byte getFirstFreeSlot();
    bool isLocked();
};

#endif

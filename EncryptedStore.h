#ifndef __PGEN__ENCRYPTED_STORE_H__
#define __PGEN__ENCRYPTED_STORE_H__

#include "Arduino.h"
#include "vendor/sha256.h"
#include "vendor/aes256_ctr.h"
#include "NoiseSource.h"
#include <EEPROM.h>

#define ENCRYPTED_STORE_KEY_SIZE 32
#define ENCRYPTED_STORE_IV_SIZE 16
#define ENCRYPTED_STORE_BLOCK_SIZE 16
#define ENCRYPTED_STORE_DATA_SIZE 64
#define ENCRYPTED_STORE_LABEL_SIZE 16

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
    void deriveKey(char *masterPassword);
    uint16_t getEncryptedEntryAddress(byte index);

public:
    void init(char *masterPassword);
    void get(byte index, char *plainText);
    void getLabel(byte index, char *label);
    void set(byte index, char *plainText, char *label);
};

#endif

#ifndef __PGEN__ENCRYPTED_STORE_H__
#define __PGEN__ENCRYPTED_STORE_H__

#include "Arduino.h"
#include <AES.h>
#include "NoiseSource.h"
#include <EEPROM.h>

#define ENCRYPTED_STORE_AES_SIZE 128

class EncryptedStore
{
private:
    byte *key;
    uint64_t setIV(uint64_t ivSeed = 0);
    byte iv[N_BLOCK];
    AES aes;

    struct EncryptedEntry
    {
        byte cipher[255];
        byte paddedCipherTextLength;
        uint64_t ivSeed;
    };

public:
    void init(byte *key);
    void get(byte index, char *plainText);
    void set(byte index, byte plainTextLength, char *plainText);
};

#endif

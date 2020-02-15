#ifndef __PGEN__ENCRYPTED_STORE_H__
#define __PGEN__ENCRYPTED_STORE_H__

#include "Arduino.h"
#include <AES.h>
#include "NoiseSource.h"

#define ENCRYPTED_STORE_AES_SIZE 128

class EncryptedStore
{
private:
    byte *key;
    void generateIV();
    byte iv[N_BLOCK];
    byte cipher[255];
    byte paddedCipherTextLength;
    AES aes;

public:
    void init(byte *key);
    void get(byte index, char *plainText);
    void set(byte index, byte plainTextLength, char *plainText);
};

#endif

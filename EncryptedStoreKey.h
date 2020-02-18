#ifndef __PGEN__ENCRYPTED_STORE_KEY_H__
#define __PGEN__ENCRYPTED_STORE_KEY_H__

#include "Arduino.h"
#include "sha256.h"

class EncryptedStoreKey
{
private:
    Sha256Class sha256;

public:
    uint8_t *key;
    void setMasterPassword(char *masterPassword);

};

#endif


#include "EncryptedStoreKey.h"

void EncryptedStoreKey::setMasterPassword(char *masterPassword)
{
    this->sha256.init();
    this->sha256.print(masterPassword);
    this->key = this->sha256.result();    
}
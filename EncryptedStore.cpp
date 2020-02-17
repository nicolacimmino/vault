#include "EncryptedStore.h"

void EncryptedStore::init(byte *key)
{
    memcpy(this->key, key, 32);
}

void EncryptedStore::get(byte index, char *plainText)
{
    EncryptedEntry encryptedEntry;
    for (byte ix = 0; ix < sizeof(encryptedEntry); ix++)
    {
        *(((unsigned char *)&encryptedEntry) + ix) = EEPROM.read(ix);
    }

    memcpy(this->iv, encryptedEntry.iv, ENCRYPTED_STORE_IV_SIZE);
    memcpy(plainText, encryptedEntry.cipher, 16);

    aes256CtrCtx_t ctx;   
    aes256CtrInit(&ctx, this->key, this->iv, ENCRYPTED_STORE_IV_SIZE);
	aes256CtrDecrypt(&ctx, plainText, 16);
}

void EncryptedStore::set(byte index, char *plainText)
{
    this->generateIV();

    EncryptedEntry encryptedEntry;
    memcpy(encryptedEntry.iv, this->iv, ENCRYPTED_STORE_IV_SIZE);
    memcpy(encryptedEntry.cipher, plainText, strlen(plainText));

    aes256CtrCtx_t ctx;   
    aes256CtrInit(&ctx, this->key, this->iv, ENCRYPTED_STORE_IV_SIZE);
	aes256CtrEncrypt(&ctx, encryptedEntry.cipher, strlen(encryptedEntry.cipher));

    for (byte ix = 0; ix < sizeof(encryptedEntry); ix++)
    {
        EEPROM.write(ix, *(((unsigned char *)&encryptedEntry) + ix));
    }
}

void EncryptedStore::generateIV()
{
    for (byte ix = 0; ix < ENCRYPTED_STORE_IV_SIZE; ix++)
    {
        while (!NoiseSource::instance()->isRandomNumberReady())
        {
            delay(1);
        }
        this->iv[ix] = NoiseSource::instance()->getRandomNumber();
    }
}
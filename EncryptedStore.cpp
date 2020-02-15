#include "EncryptedStore.h"

void EncryptedStore::init(byte *key)
{
    this->key = key;
}

void EncryptedStore::get(byte index, char *plainText)
{
    EncryptedEntry encryptedEntry;

    for (uint32_t ix = 0; ix < sizeof(encryptedEntry); ix++)
    {
        *(((unsigned char *)&encryptedEntry) + 1) = EEPROM.read(ix);
    }

    this->setIV(encryptedEntry.ivSeed);
    this->aes.do_aes_decrypt(encryptedEntry.cipher, encryptedEntry.paddedCipherTextLength, plainText, this->key, ENCRYPTED_STORE_AES_SIZE, this->iv);
}

void EncryptedStore::set(byte index, byte plainTextLength, char *plainText)
{
    EncryptedEntry encryptedEntry;

    encryptedEntry.ivSeed = this->setIV();
    this->aes.do_aes_encrypt(plainText, plainTextLength, encryptedEntry.cipher, this->key, ENCRYPTED_STORE_AES_SIZE, this->iv);
    encryptedEntry.paddedCipherTextLength = plainTextLength + N_BLOCK - plainTextLength % N_BLOCK;

    for (uint32_t ix = 0; ix < sizeof(encryptedEntry); ix++)
    {
        EEPROM.write(ix, *(((unsigned char *)&encryptedEntry) + 1));
    }
}

uint64_t EncryptedStore::setIV(uint64_t ivSeed = 0)
{
    if (ivSeed == 0)
    {
        while (!NoiseSource::instance()->isRandomNumberReady())
        {
            delay(1);
        }
        ivSeed = NoiseSource::instance()->getRandomNumber();
    }

    this->aes.set_IV(ivSeed);
    this->aes.get_IV(this->iv);

    return ivSeed;
}
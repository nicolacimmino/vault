#include "EncryptedStore.h"

void EncryptedStore::init(byte *key)
{
    this->key = key;
}

void EncryptedStore::get(byte index, char *plainText)
{
    byte iv[N_BLOCK];
    this->aes.set_IV(this->iv);
    this->aes.get_IV(iv);
    this->aes.set_IV(this->iv);
    this->aes.do_aes_decrypt(this->cipher, this->paddedCipherTextLength, plainText, this->key, ENCRYPTED_STORE_AES_SIZE, iv);
}

void EncryptedStore::set(byte index, byte plainTextLength, char *plainText)
{
    byte iv[N_BLOCK];
    this->generateIV();
    this->aes.set_IV(this->iv);
    this->aes.get_IV(iv);
    this->aes.do_aes_encrypt(plainText, plainTextLength, this->cipher, this->key, ENCRYPTED_STORE_AES_SIZE, iv);
    this->paddedCipherTextLength = plainTextLength + N_BLOCK - plainTextLength % N_BLOCK;
}

void EncryptedStore::generateIV()
{
    byte ix = 0;
    while (ix < N_BLOCK)
    {
        while (!NoiseSource::instance()->isRandomNumberReady())
        {
            delay(1);
        }

        uint32_t randomNumber = NoiseSource::instance()->getRandomNumber();

        for (byte i = 0; i < 4; i++)
        {
            this->iv[ix] = randomNumber & 0xFF;
            randomNumber = randomNumber >> 8;
            ix++;
        }
    }
}
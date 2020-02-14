#include "EncryptedStore.h"

void EncryptedStore::init(byte *key)
{
    this->key = key;    
}

void EncryptedStore::get(byte index, char *plainText)
{
}

void EncryptedStore::set(byte index, byte plainTextLength, char *plainText)
{
    this->generateIV();
}

void EncryptedStore::generateIV()
{
    Serial.println("Initialising IV");
    byte ix = 0;
    while (ix < N_BLOCK)
    {
        while (!NoiseSource::instance()->isRandomNumberReady())
        {
            delay(1);
        }

        uint32_t randomNumber = NoiseSource::instance()->getRandomNumber();

        for(byte i=0; i<4; i++) {
            this->iv[ix] = randomNumber & 0xFF;
            randomNumber = randomNumber >> 8;
            ix++;
        }
    }
    Serial.print("IV:");
    for(ix=0; ix< N_BLOCK; ix++) {
        Serial.print(this->iv[ix], HEX);
    }
    Serial.println("");
}
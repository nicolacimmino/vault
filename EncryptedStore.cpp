#include "EncryptedStore.h"

void EncryptedStore::unlock(char *masterPassword)
{
    this->deriveKey(masterPassword);
    this->locked = false;
}

void EncryptedStore::lock()
{
    memset(this->key, 0, ENCRYPTED_STORE_KEY_SIZE);
    this->locked = true;
}

bool EncryptedStore::isLocked()
{
    return this->locked;
}

void EncryptedStore::wipe(byte index)
{
    EncryptedEntry encryptedEntry;
    memset(&encryptedEntry, 0, sizeof(EncryptedEntry));

    EEPROM.put(this->getEncryptedEntryAddress(index), encryptedEntry);
}

void EncryptedStore::deriveKey(char *masterPassword)
{
    Sha256Class sha256;
    sha256.init();
    sha256.print(masterPassword);
    memcpy(this->key, sha256.result(), ENCRYPTED_STORE_KEY_SIZE);
}

byte EncryptedStore::getFirstFreeSlot()
{
    for (byte ix = 0; ix < ENCRYPTED_STORE_MAX_ENTRIES; ix++)
    {
        char label[ENCRYPTED_STORE_LABEL_SIZE];
        this->getLabel(ix, label);

        if (strlen(label) == 0)
        {
            return ix;
        }
    }

    return ENCRYPTED_STORE_FULL;
}

void EncryptedStore::get(byte index, char *plainText)
{
    EncryptedEntry encryptedEntry;
    EEPROM.get(this->getEncryptedEntryAddress(index), encryptedEntry);

    aes256CtrCtx_t ctx;
    aes256CtrInit(&ctx, this->key, encryptedEntry.iv, ENCRYPTED_STORE_IV_SIZE);

    for (int ix = 0; ix < ENCRYPTED_STORE_DATA_SIZE; ix += ENCRYPTED_STORE_BLOCK_SIZE)
    {
        aes256CtrDecrypt(&ctx, encryptedEntry.cipher + ix, ENCRYPTED_STORE_BLOCK_SIZE);
    }

    memcpy(plainText, encryptedEntry.cipher, ENCRYPTED_STORE_DATA_SIZE);
}

void EncryptedStore::getLabel(byte index, char *label)
{
    EncryptedEntry encryptedEntry;
    EEPROM.get(this->getEncryptedEntryAddress(index), encryptedEntry);

    memcpy(label, encryptedEntry.label, ENCRYPTED_STORE_LABEL_SIZE);
}

void EncryptedStore::set(byte index, char *plainText, char *label)
{
    EncryptedEntry encryptedEntry;
    this->generateIV(encryptedEntry.iv);

    aes256CtrCtx_t ctx;
    aes256CtrInit(&ctx, this->key, encryptedEntry.iv, ENCRYPTED_STORE_IV_SIZE);

    this->safeStringCopy(encryptedEntry.cipher, plainText, ENCRYPTED_STORE_DATA_SIZE);
    this->safeStringCopy(encryptedEntry.label, label, ENCRYPTED_STORE_LABEL_SIZE);

    for (int ix = 0; ix < ENCRYPTED_STORE_DATA_SIZE; ix += ENCRYPTED_STORE_BLOCK_SIZE)
    {
        Serial.print((char)(encryptedEntry.cipher[ix]));
        aes256CtrEncrypt(&ctx, encryptedEntry.cipher + ix, ENCRYPTED_STORE_BLOCK_SIZE);
    }

    EEPROM.put(this->getEncryptedEntryAddress(index), encryptedEntry);

    memset(&encryptedEntry, 0, sizeof(encryptedEntry));
    memset(&ctx, 0, sizeof(ctx));
}

void EncryptedStore::safeStringCopy(char *destination, char *source, byte destinationSize)
{
    memset(destination, 0, destinationSize);
    memcpy(destination, source, min(strlen(source), destinationSize - 1));
}

void EncryptedStore::generateIV(byte *iv)
{
    for (byte ix = 0; ix < ENCRYPTED_STORE_IV_SIZE; ix++)
    {
        while (!NoiseSource::instance()->isRandomNumberReady())
        {
            delay(1);
        }
        iv[ix] = NoiseSource::instance()->getRandomNumber();
    }
}

uint16_t EncryptedStore::getEncryptedEntryAddress(byte index)
{
    return index * sizeof(EncryptedEntry);
}

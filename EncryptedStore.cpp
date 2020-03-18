#include "EncryptedStore.h"

void EncryptedStore::unlock(SafeBuffer *masterPassword)
{
    Sha256Class sha256;
    sha256.init();
    sha256.print(masterPassword->getBuffer());
    memcpy(this->key, sha256.result(), ENCRYPTED_STORE_KEY_SIZE);

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

void EncryptedStore::fullWipe()
{
    for (uint16_t ix = 0; ix < ENCRYPTED_STORE_EEPROM_SIZE; ix++)
    {
        EEPROM.write(ix, 0);
    }

    this->lock();
}

bool EncryptedStore::isPositionFree(byte index)
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];
    return !this->getLabel(index, label);
}

void EncryptedStore::get(byte index, SafeBuffer *plainText)
{
    EncryptedEntry encryptedEntry;
    EEPROM.get(this->getEncryptedEntryAddress(index), encryptedEntry);

    aes256CtrCtx_t ctx;
    aes256CtrInit(&ctx, this->key, encryptedEntry.iv, ENCRYPTED_STORE_IV_SIZE);

    for (int ix = 0; ix < ENCRYPTED_STORE_DATA_SIZE; ix += ENCRYPTED_STORE_BLOCK_SIZE)
    {
        aes256CtrDecrypt(&ctx, encryptedEntry.cipher + ix, ENCRYPTED_STORE_BLOCK_SIZE);
    }

    plainText->strcpy(encryptedEntry.cipher);

    memset(encryptedEntry.cipher, 0, sizeof(encryptedEntry));
}

void EncryptedStore::getTokens(byte index, char *tokensList, SafeBuffer *plainText)
{
    byte ix = 0;
    SafeBuffer *password = new SafeBuffer(ENCRYPTED_STORE_DATA_SIZE);

    this->get(index, password);

    char *token = strtok(tokensList, ",");
    while (token != NULL)
    {
        int tokenIndex = atoi(token) - 1;
        plainText->setChar(ix, password->getChar(tokenIndex));
        token = strtok(NULL, ",");
        ix++;
    }

    delete password;
}

bool EncryptedStore::getLabel(byte index, char *label)
{
    if (index > ENCRYPTED_STORE_MAX_ENTRIES)
    {
        return false;
    }

    EncryptedEntry encryptedEntry;
    EEPROM.get(this->getEncryptedEntryAddress(index), encryptedEntry);

    if (strlen(encryptedEntry.label) == 0)
    {
        return false;
    }

    memcpy(label, encryptedEntry.label, ENCRYPTED_STORE_LABEL_SIZE);

    return true;
}

void EncryptedStore::set(byte index, SafeBuffer *plainText, SafeBuffer *label)
{
    EncryptedEntry encryptedEntry;
    this->generateIV(encryptedEntry.iv);

    aes256CtrCtx_t ctx;
    aes256CtrInit(&ctx, this->key, encryptedEntry.iv, ENCRYPTED_STORE_IV_SIZE);

    this->safeStringCopy(encryptedEntry.cipher, plainText->getBuffer(), plainText->getBufferSize());
    this->safeStringCopy(encryptedEntry.label, label->getBuffer(), label->getBufferSize());

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

byte *EncryptedStore::getFirmwareFingerprint()
{
    Sha256Class sha256;
    sha256.initHmac(this->key, ENCRYPTED_STORE_KEY_SIZE);

    for (uint32_t address = 0; address < 32768; address++)
    {
        sha256.write((byte)pgm_read_byte(address));
    }

    memcpy(this->fwFingerPrint, sha256.result(), ENCRYPTED_STORE_FW_FINGERPRINT_SIZE);

    return this->fwFingerPrint;
}

int EncryptedStore::getKeyFingerprint()
{
    CRC32 crc;

    for (byte ix = 0; ix < ENCRYPTED_STORE_KEY_SIZE / 4; ix++)
    {
        crc.update(this->key[ix]);
    }

    return crc.finalize() % 10000;
}
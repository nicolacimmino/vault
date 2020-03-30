#include "EncryptedStore.h"

void EncryptedStore::unlock(char *masterPassword)
{
    Sha256Class *sha256 = new Sha256Class();
    sha256->init();
    sha256->print(masterPassword);
    memcpy(this->key, sha256->result(), ENCRYPTED_STORE_KEY_SIZE);

    this->locked = false;

    delete sha256;
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
    for (byte ix = 0; ix < sizeof(EncryptedEntry); ix++)
    {
        EEPROM.write(this->getEncryptedEntryAddress(index) + ix, 0);
    }
}

bool EncryptedStore::isPositionFree(byte index)
{
    return EEPROM.read(this->getEncryptedEntryAddress(index)) == 0;
}

void EncryptedStore::get(byte index, char *plainText)
{
    EncryptedEntry *encryptedEntry = new EncryptedEntry();
    EEPROM.get(this->getEncryptedEntryAddress(index), *encryptedEntry);

    aes256CtrCtx_t *ctx = new aes256CtrCtx_t();
    aes256CtrInit(ctx, this->key, encryptedEntry->iv, ENCRYPTED_STORE_IV_SIZE);

    for (int ix = 0; ix < ENCRYPTED_STORE_DATA_SIZE; ix += ENCRYPTED_STORE_BLOCK_SIZE)
    {
        aes256CtrDecrypt(ctx, encryptedEntry->cipher + ix, ENCRYPTED_STORE_BLOCK_SIZE);
    }

    strcpy(plainText, encryptedEntry->cipher);

    memset(encryptedEntry->cipher, 0, sizeof(*encryptedEntry));

    delete ctx;
    delete encryptedEntry;
}

void EncryptedStore::getTokens(byte index, char *tokensList, char *plainText)
{
    byte ix = 0;
    char *password = new char[ENCRYPTED_STORE_DATA_SIZE];

    this->get(index, password);

    char *token = strtok(tokensList, ",");
    while (token != NULL)
    {
        int tokenIndex = atoi(token) - 1;
        plainText[ix] = password[tokenIndex];
        token = strtok(NULL, ",");
        ix++;
    }

    delete password;
}

bool EncryptedStore::getLabel(byte index, char *label)
{
    memset(label, 0, ENCRYPTED_STORE_LABEL_SIZE);

    if (index > ENCRYPTED_STORE_MAX_ENTRIES)
    {
        return false;
    }

    EncryptedEntry *encryptedEntry = new EncryptedEntry();
    EEPROM.get(this->getEncryptedEntryAddress(index), *encryptedEntry);

    if (strlen(encryptedEntry->label) != 0)
    {
        memcpy(label, encryptedEntry->label, ENCRYPTED_STORE_LABEL_SIZE);
    }

    delete encryptedEntry;

    return strlen(label) != 0;
}

void EncryptedStore::set(byte index, char *plainText, char *label)
{
    EncryptedEntry *encryptedEntry = new EncryptedEntry();
    this->generateIV(encryptedEntry->iv);

    aes256CtrCtx_t *ctx = new aes256CtrCtx_t();
    aes256CtrInit(ctx, this->key, encryptedEntry->iv, ENCRYPTED_STORE_IV_SIZE);

    strcpy(encryptedEntry->cipher, plainText);
    strcpy(encryptedEntry->label, label);

    for (int ix = 0; ix < ENCRYPTED_STORE_DATA_SIZE; ix += ENCRYPTED_STORE_BLOCK_SIZE)
    {
        Serial.print((char)(encryptedEntry->cipher[ix]));
        aes256CtrEncrypt(ctx, encryptedEntry->cipher + ix, ENCRYPTED_STORE_BLOCK_SIZE);
    }

    EEPROM.put(this->getEncryptedEntryAddress(index), *encryptedEntry);

    aes256CtrClean(ctx);

    memset(encryptedEntry, 0, sizeof(encryptedEntry));

    delete encryptedEntry;
    delete ctx;
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

int EncryptedStore::getKeyFingerprint()
{
    CRC32 *crc = new CRC32();

    for (byte ix = 0; ix < ENCRYPTED_STORE_KEY_SIZE / 4; ix++)
    {
        crc->update(this->key[ix]);
    }

    int fingerprint = crc->finalize() % 10000;

    delete crc;

    return fingerprint;
}
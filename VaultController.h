
#ifndef __VAULT_CONTROLLER_H__
#define __VAULT_CONTROLLER_H__

#include "hardware.h"
#include "options.h"
#include <Functor.h>
#include "Terminal.h"
#include "EncryptedStore.h"
#include <Keyboard.h>

#define MASTER_PASSWORD_MAX_SIZE 64

class VaultController
{
private:
    Terminal terminal;
    EncryptedStore encryptedStore;
    char clipboard[ENCRYPTED_STORE_DATA_SIZE];
    byte selectedPasswordIndex;
    
    void unlockEncryptedStore();
    void displayPasswordSelectionMenu();
    void displayPasswordActionMenu();

public:    
    void loop();
    void begin();
    void addPassword();
    void wipePassword();
    void actOnPassword(byte action);
    void resetTerminal();
    void lockStore();
    void selectPassword(byte index);
};

#endif
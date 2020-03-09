
#ifndef __VAULT_CONTROLLER_H__
#define __VAULT_CONTROLLER_H__

#include "hardware.h"
#include "options.h"
#include "Terminal.h"
#include "EncryptedStore.h"
#include <Keyboard.h>

#define MASTER_PASSWORD_MAX_SIZE 64

extern void invoke_vaultController_addPassword();
extern void invoke_vaultController_wipePassword();
extern void invoke_vaultController_lockStore();
extern void invoke_vaultController_selectPassword(byte index);
extern void invoke_vaultController_actOnPassword(byte action);
extern void invoke_vaultController_resetTerminal();

class VaultController
{
private:
    Terminal terminal;
    EncryptedStore encryptedStore;
    char clipboard[ENCRYPTED_STORE_DATA_SIZE];
    byte selectedPasswordIndex;
    static VaultController *instance;

    void unlockEncryptedStore();
    void displayPasswordSelectionMenu();
    void displayPasswordActionMenu();

public:
    static VaultController *getInstance();
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
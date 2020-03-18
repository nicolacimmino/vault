
#ifndef __VAULT_CONTROLLER_H__
#define __VAULT_CONTROLLER_H__

#include "hardware.h"
#include "options.h"
#include <Functor.h>
#include "Terminal.h"
#include "EncryptedStore.h"
#include <Keyboard.h>
#include "NotificationLight.h"

class VaultController
{
private:
    Terminal terminal;
    NotificationLight notificationLight;
    EncryptedStore encryptedStore;
    SafeBuffer *clipboard;
    byte selectedPasswordIndex;

    void unlockEncryptedStore();
    void lockEnctryptedStore();
    void displayPasswordSelectionMenu();
    void displayPasswordActionMenu();
    void showOptionsScreen();
    void processOptionsSelection(byte action);
    
public:
    VaultController();
    ~VaultController();
    void loop();    
    void addPassword();
    void wipePassword();
    void selectPassword(byte index);
    void retrievePassword(byte action);
    void resetTerminal();    
    void backup();
    
};

#endif

#ifndef __VAULT_CONTROLLER_H__
#define __VAULT_CONTROLLER_H__

#include "hardware.h"
#include <Functor.h>
#include "Terminal.h"
#include "EncryptedStore.h"
#include <Keyboard.h>
#include "NotificationController.h"
#include "Service.h"
#include "BackupService.h"
#include "FullWipeService.h"
#include "RestoreBackupService.h"
#include "RetrievePasswordService.h"
#include "AddPasswordService.h"
#include "UnlockEncryptedStoreService.h"

class VaultController
{
private:
    Terminal *terminal;
    EncryptedStore *encryptedStore;
    NotificationController *notificationController;
    
    byte selectedPasswordIndex;

    void startStoreUnlockService();
    void lockEnctryptedStore();
    void displayPasswordSelectionMenu();
    void displayPasswordActionMenu();
    void displayOptionsMenu();
    void processOptionsSelection(byte action);
    void setLedStatus(bool green, bool yellow, bool red);
    void setTime();
    void backup();
    void restoreBackup();
    void fullWipe();
    Service *activeService = NULL;

public:
    VaultController();
    void loop();
    void addPassword();
    void deletePassword();
    void selectPassword(byte index);
    void retrievePassword(byte action);
    void resetTerminal();    
};

#endif
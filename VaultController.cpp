
#include "VaultController.h"

VaultController::VaultController()
{
    this->encryptedStore = new EncryptedStore();
    this->terminal = new Terminal(this->encryptedStore);
    this->notificationController = new NotificationController();

    this->notificationController->setStoreLocked(true, 0);

    pinMode(BUTTON_SENSE, INPUT_PULLUP);

    this->terminal->init();

    this->terminal->setResetCallback(makeFunctor((Functor0 *)0, *this, &VaultController::resetVault));

    this->resetVault();

    Keyboard.begin();
}

void VaultController::unlockEncryptedStore()
{
    this->activeService = new UnlockEncryptedStoreService(
        this->terminal,
        this->encryptedStore,
        this->notificationController,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    this->activeService->start();
}

void VaultController::lockEnctryptedStore()
{
    this->encryptedStore->lock();

    this->notificationController->setStoreLocked(true, 0);

    this->terminal->clearScreen();
    this->terminal->printStatusMessage(TXT_STATUS_LOCKED);
}

/**
 * Select password to perform actions on.
 */
void VaultController::selectPassword(byte index)
{
    if (index >= ENCRYPTED_STORE_MAX_ENTRIES || this->encryptedStore->isPositionFree(index))
    {
        this->displayPasswordSelectionMenu();
        return;
    }

    this->selectedPasswordIndex = index;

    this->displayPasswordActionMenu();
}

/**
 * Display a menu to select one of the available passwords.
 */
void VaultController::displayPasswordSelectionMenu()
{
    char *label = new char[ENCRYPTED_STORE_LABEL_SIZE];

    this->terminal->clearScreen();

    for (byte menuPosition = 0; menuPosition < ENCRYPTED_STORE_MAX_ENTRIES; menuPosition++)
    {
        this->encryptedStore->getLabel(menuPosition, label, TXT_EMPTY_LABEL);
        this->terminal->printMenuEntry(menuPosition, label);
    }

    this->terminal->clearHotkeys();
    this->terminal->addHotkey(HOTKEY_ADD, makeFunctor((Functor0 *)0, *this, &VaultController::addPassword));
    this->terminal->addHotkey(HOTKEY_DEL, makeFunctor((Functor0 *)0, *this, &VaultController::deletePassword));
    this->terminal->addHotkey(HOTKEY_KFP, makeFunctor((Functor0 *)0, *this->terminal, &Terminal::showKeyFingerprint));
    this->terminal->addHotkey(HOTKEY_LCK, makeFunctor((Functor0 *)0, *this, &VaultController::lockEnctryptedStore));
    this->terminal->addHotkey(HOTKEY_OPT, makeFunctor((Functor0 *)0, *this, &VaultController::displayOptionsMenu));
    this->terminal->printStatusMessage(MENU_HOTKEYS);

    this->terminal->setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::selectPassword));

    delete label;
}

void VaultController::displayOptionsMenu()
{
    this->terminal->clearCanvas();
    this->terminal->setMenuCallback(NULL);

    this->terminal->printMenuEntry(MENU_L1_FIRST_POS,
                                   TXT_MENU_BACKUP,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::backup));

    this->terminal->printMenuEntry(MENU_L1_FIRST_POS + 1,
                                   TXT_MENU_BACKUP_RESTORE,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::restoreBackup));

    this->terminal->printMenuEntry(MENU_L1_FIRST_POS + 2,
                                   TXT_MENU_FULL_WIPE,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::fullWipe));

    this->terminal->printMenuEntry(MENU_L1_FIRST_POS + 3,
                                   TXT_MENU_BACK,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    this->terminal->clearHotkeys();
    this->terminal->printStatusMessage(TXT_MENU_OPTIONS);
}

void VaultController::displayPasswordActionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];
    this->encryptedStore->getLabel(this->selectedPasswordIndex, label, TXT_EMPTY_LABEL);

    char statusMessage[TERMINAL_WIDTH];
    sprintf(statusMessage, TXT_SELECTED_PASSWORD_LABEL, label);
    this->terminal->printStatusMessage(statusMessage);

    this->terminal->clearHotkeys();
    this->terminal->setMenuCallback(NULL);

    this->terminal->printMenuEntry(MENU_L2_FIRST_POS,
                                   TXT_MENU_PASSWORD_TO_CLIPBOARD,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::retrievePasswordFull));

    this->terminal->printMenuEntry(MENU_L2_FIRST_POS + 1,
                                   TXT_MENU_PASSWORD_TO_CLIPBOARD_PARTIAL,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::retrievePasswordPartial));

    this->terminal->printMenuEntry(MENU_L2_FIRST_POS + 2,
                                   TXT_MENU_PASSWORD_SHOW,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::retrievePasswordShow));

    this->terminal->printMenuEntry(MENU_L2_FIRST_POS + 3,
                                   TXT_MENU_BACK,
                                   VT_FOREGROUND_WHITE,
                                   makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));
}

void VaultController::resetVault()
{
    if (this->activeService)
    {
        delete this->activeService;
        this->activeService = NULL;
    }

    this->lockEnctryptedStore();
}

void VaultController::fullWipe()
{
    this->activeService = new FullWipeService(
        this->terminal,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayOptionsMenu));

    this->activeService->start();
}

void VaultController::retrievePasswordShow()
{
    this->retrievePassword(RETRIEVE_PASSWORD_ACTION_SHOW);
}

void VaultController::retrievePasswordFull()
{
    this->retrievePassword(RETRIEVE_PASSWORD_ACTION_FULL);
}

void VaultController::retrievePasswordPartial()
{
    this->retrievePassword(RETRIEVE_PASSWORD_ACTION_PARTIAL);
}

void VaultController::retrievePassword(byte action)
{
    this->activeService = new RetrievePasswordService(
        this->terminal,
        this->encryptedStore,
        this->notificationController,
        this->selectedPasswordIndex,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    this->activeService->start(action);
}

/**
 * Delete a password.
 * 
 * Asks the user the position to wipe.
 */
void VaultController::deletePassword()
{
    this->activeService = new DeletePasswordService(
        this->terminal,
        this->encryptedStore,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    this->activeService->start();
}

void VaultController::backup()
{
    this->activeService = new BackupService(
        this->terminal,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayOptionsMenu));

    this->activeService->start();
}

void VaultController::restoreBackup()
{
    this->activeService = new RestoreBackupService(
        this->terminal,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayOptionsMenu));

    this->activeService->start();
}

/**
 * Add a password to the store.
 * 
 * Asks the user the position, label and actual password and adds it to the store.
 */
void VaultController::addPassword()
{
    this->activeService = new AddPasswordService(
        this->terminal,
        this->encryptedStore,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    this->activeService->start();
}

void VaultController::loop()
{
    if (this->activeService)
    {
        this->activeService->loop();

        if (!this->activeService->isRunning())
        {
            delete this->activeService;
            this->activeService = NULL;
        }
    }

    if (encryptedStore->isLocked())
    {
        this->unlockEncryptedStore();
    }

    this->terminal->loop(this->activeService != NULL);
}

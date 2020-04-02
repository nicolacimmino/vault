
#include "VaultController.h"

VaultController::VaultController()
{
    this->encryptedStore = new EncryptedStore();
    this->terminal = new Terminal(this->encryptedStore);
    this->notificationController = new NotificationController();

    pinMode(BUTTON_SENSE, INPUT_PULLUP);

    this->terminal->init();

    this->terminal->setResetCallback(makeFunctor((Functor0 *)0, *this, &VaultController::resetVault));

    this->resetVault();

    Keyboard.begin();
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
    this->terminal->addHotkey('a', makeFunctor((Functor0 *)0, *this, &VaultController::addPassword));
    this->terminal->addHotkey('d', makeFunctor((Functor0 *)0, *this, &VaultController::deletePassword));
    this->terminal->addHotkey('k', makeFunctor((Functor0 *)0, *this->terminal, &Terminal::showKeyFingerprint));
    this->terminal->addHotkey('l', makeFunctor((Functor0 *)0, *this, &VaultController::lockEnctryptedStore));
    this->terminal->addHotkey('o', makeFunctor((Functor0 *)0, *this, &VaultController::displayOptionsMenu));
    this->terminal->printStatusMessage(" ALT+A Add  |  ALT+D Delete  |  ALT + K KFP  |  ALT+L Lock  |  ALT+O Options");

    this->terminal->setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::selectPassword));

    delete label;
}

void VaultController::displayOptionsMenu()
{
    this->terminal->clearCanvas();
    this->terminal->printMenuEntry(0, "Backup", VT_FOREGROUND_WHITE, makeFunctor((Functor0 *)0, *this, &VaultController::backup));
    this->terminal->printMenuEntry(1, "Restore Backup", VT_FOREGROUND_WHITE, makeFunctor((Functor0 *)0, *this, &VaultController::restoreBackup));
    this->terminal->printMenuEntry(2, "Full Wipe", VT_FOREGROUND_WHITE, makeFunctor((Functor0 *)0, *this, &VaultController::fullWipe));
    this->terminal->printMenuEntry(3, "Back", VT_FOREGROUND_WHITE, makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    this->terminal->clearHotkeys();
    this->terminal->printStatusMessage(" Options ");

    this->terminal->setMenuCallback(NULL);
}

void VaultController::displayPasswordActionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];
    this->encryptedStore->getLabel(this->selectedPasswordIndex, label, TXT_EMPTY_LABEL);

    char statusMessage[TERMINAL_WIDTH];
    sprintf(statusMessage, TXT_SELECTED_PASSWORD_LABEL, label);

    this->terminal->clearHotkeys();
    this->terminal->printStatusMessage(statusMessage);

    this->terminal->printMenuEntry(TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION, TXT_MENU_PASSWORD_TO_CLIPBOARD);
    this->terminal->printMenuEntry(TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION + 1, TXT_MENU_PASSWORD_TO_CLIPBOARD_PARTIAL);
    this->terminal->printMenuEntry(TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION + 2, TXT_MENU_BACK);

    this->terminal->setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::retrievePassword));
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

/**
 * Unlock the store.
 * 
 * Get the user master password and use it to unlock the store.
 */
void VaultController::startStoreUnlockService()
{
    this->activeService = new UnlockEncryptedStoreService(
        this->terminal,
        this->encryptedStore,
        this->notificationController,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    this->activeService->start();
}

void VaultController::fullWipe()
{
    this->activeService = new FullWipeService(
        this->terminal,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayOptionsMenu));

    this->activeService->start();
}

void VaultController::retrievePassword(byte action)
{
    action -= TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION;

    this->activeService = new RetrievePasswordService(
        this->terminal,
        this->encryptedStore,
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

/**
 * Lock the store and wipe the clipboard.
 */
void VaultController::lockEnctryptedStore()
{
    this->encryptedStore->lock();

    this->notificationController->setStoreLocked(true);

    this->terminal->clearScreen();
    this->terminal->printStatusMessage(" Locked.");
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
        this->startStoreUnlockService();
    }

    this->terminal->loop(this->activeService != NULL);
}

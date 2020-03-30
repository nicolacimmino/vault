
#include "VaultController.h"

VaultController::VaultController()
{
    this->terminal = new Terminal();
    this->encryptedStore = new EncryptedStore();
    this->notificationController = new NotificationController();

    pinMode(BUTTON_SENSE, INPUT_PULLUP);

    this->resetTerminal();

    Keyboard.begin();

}

/**
 * Unlock the store.
 * 
 * Get the user master password and use it to unlock the store.
 */
void VaultController::unlockEncryptedStore()
{
    char *masterPassword = new char[ENCRYPTED_STORE_MASTER_PASSWORD_MAX_SIZE];

    if (this->terminal->readString(TXT_ENTER_MASTER_PASSWORD, masterPassword, ENCRYPTED_STORE_MASTER_PASSWORD_MAX_SIZE, TXT_PASSWORD_MASK, TERMINAL_FIRST_CANVAS_LINE + 2, 2))
    {
        this->encryptedStore->unlock(masterPassword);

        this->notificationController->setStoreLocked(false);
    }

    delete masterPassword;
}

/**
 * Add a password to the store.
 * 
 * Asks the user the position, label and actual password and adds it to the store.
 */
void VaultController::addPassword()
{
    this->terminal->print("Select position: ", TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 2);

    byte selectedIndex = this->terminal->waitKeySelection('a', 'a' + ENCRYPTED_STORE_MAX_ENTRIES);

    if (selectedIndex != TERMINAL_OPERATION_ABORTED)
    {
        this->terminal->clearCanvas();

        char *label = new char[ENCRYPTED_STORE_LABEL_SIZE];
        char *password = new char[ENCRYPTED_STORE_DATA_SIZE];

        this->terminal->readString("Enter label: ", label, ENCRYPTED_STORE_LABEL_SIZE, 0, TERMINAL_FIRST_CANVAS_LINE + 2, 2);
        this->terminal->readString("Enter password: ", password, ENCRYPTED_STORE_DATA_SIZE, '*', TERMINAL_FIRST_CANVAS_LINE + 3, 2);

        this->terminal->printStatusMessage(" Enctrypting......");
        this->encryptedStore->set(selectedIndex, password, label);

        delete label;
        delete password;
    }

    this->displayPasswordSelectionMenu();
}

/**
 * Wipe a password.
 * 
 * Asks the user the position to wipe.
 */
void VaultController::wipePassword()
{
    this->terminal->print("Select position to wipe: ", TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 5);

    byte selectedIndex = this->terminal->waitKeySelection('a', 'a' + ENCRYPTED_STORE_MAX_ENTRIES);

    if (this->terminal->askYesNoQuestion(TXT_WIPE_PASSWORD_CONFIRMATION, true))
    {
        this->encryptedStore->wipe(selectedIndex);
    }

    this->displayPasswordSelectionMenu();
}

/**
 * Lock the store and wipe the clipboard.
 */
void VaultController::lockEnctryptedStore()
{
    this->encryptedStore->lock();

    this->notificationController->setStoreLocked(true);
}

/**
 * Select password to perform actions on.
 */
void VaultController::selectPassword(byte index)
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

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
        if (!this->encryptedStore->getLabel(menuPosition, label))
        {
            strcpy(label, "---");
        }

        this->terminal->printMenuEntry(menuPosition, label);
    }

    this->terminal->clearHotkeys();
    this->terminal->addHotkey('a', makeFunctor((Functor0 *)0, *this, &VaultController::addPassword));
    this->terminal->addHotkey('w', makeFunctor((Functor0 *)0, *this, &VaultController::wipePassword));
    this->terminal->addHotkey('l', makeFunctor((Functor0 *)0, *this, &VaultController::lockEnctryptedStore));
    this->terminal->addHotkey('o', makeFunctor((Functor0 *)0, *this, &VaultController::displayOptionsMenu));
    this->terminal->printStatusMessage(" ALT+A Add  |  ALT+W Wipe Password  |  ALT+L Lock  |  ALT+O Options");

    this->terminal->setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::selectPassword));

    delete label;
}

void VaultController::displayOptionsMenu()
{
    this->terminal->clearCanvas();
    this->terminal->printMenuEntry(0, "Backup");
    this->terminal->printMenuEntry(1, "Full Wipe");
    this->terminal->printMenuEntry(2, "Back");

    this->terminal->setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::processOptionsSelection));
}

void VaultController::processOptionsSelection(byte action)
{
    switch (action)
    {
    case 0:
        this->backup();
        break;
    case 1:
        this->fullWipe();
        break;
    case 2:
        this->displayPasswordSelectionMenu();
        break;
    default:
        return;
    }
}

void VaultController::fullWipe()
{
    this->runningService = new FullWipeService(
        this->terminal,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::resetTerminal));

    if (!this->runningService->start())
    {
        this->displayOptionsMenu();
    }
}

void VaultController::retrievePassword(byte action)
{
    action -= TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION;

    if (action != 0 && action != 1)
    {
        this->displayPasswordSelectionMenu();
        return;
    }

    char *clipboard = new char[ENCRYPTED_STORE_DATA_SIZE];

    if (action == 0)
    {
        this->encryptedStore->get(selectedPasswordIndex, clipboard);
    }

    if (action == 1)
    {
        char *buffer = new char[TERMINAL_WIDTH];

        this->terminal->readString(TXT_ENTER_TOKENS_POS, buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 4, TERMINAL_RIGHT_HALF_FIRST_COLUMN);
        this->encryptedStore->getTokens(selectedPasswordIndex, buffer, clipboard);

        delete buffer;
    }

    this->terminal->alert("Ready. Press button to type.");

    while (strlen(clipboard) > 0)
    {
        if (digitalRead(BUTTON_SENSE) == LOW)
        {
            for (byte ix = 0; ix < strlen(clipboard); ix++)
            {
                Keyboard.print(clipboard[ix]);
                delay(300);
            }

            memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
        }

        this->loop();
    }

    delete clipboard;

    this->displayPasswordSelectionMenu();
}

void VaultController::displayPasswordActionMenu()
{
    this->terminal->printMenuEntry(TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION, "Copy to clipboard");
    this->terminal->printMenuEntry(TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION + 1, "Partial copy to clipboard");
    this->terminal->printMenuEntry(TERMINAL_SECOND_LEVEL_MENU_FIRST_POSITION + 2, "Back");

    this->terminal->setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::retrievePassword));
}

void VaultController::resetTerminal()
{
    Serial.begin(9600);

    if (this->runningService)
    {
        delete this->runningService;
        this->runningService = NULL;
    }

    this->lockEnctryptedStore();

    while (!Serial)
    {
        ;
    }
    delay(500);

    this->terminal->init(&Serial);

    this->terminal->setResetCallback(makeFunctor((Functor0 *)0, *this, &VaultController::resetTerminal));
}

void VaultController::backup()
{
    this->runningService = new BackupService(
        this->terminal,
        makeFunctor((Functor1<byte> *)0, *this->terminal, &Terminal::showProgress),
        makeFunctor((Functor0 *)0, *this, &VaultController::displayPasswordSelectionMenu));

    if (!this->runningService->start())
    {
        this->displayOptionsMenu();
    }
}

void VaultController::loop()
{
    if (this->runningService)
    {
        this->runningService->loop();

        if (!this->runningService->isRunning())
        {
            delete this->runningService;
            this->runningService = NULL;
        }
    }
    
    this->terminal->setLclIndicator(encryptedStore->isLocked());

    if (encryptedStore->isLocked())
    {
        this->notificationController->setStoreLocked(true);
        this->terminal->clearScreen();
        this->terminal->printStatusMessage(" Locked.");
        this->unlockEncryptedStore();
        this->displayPasswordSelectionMenu();
        this->terminal->setKeyFingerprint(encryptedStore->getKeyFingerprint());
        this->notificationController->setStoreLocked(false);
    }

    this->terminal->loop();
}

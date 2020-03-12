
#include "VaultController.h"

/**
 * Unlock the store.
 * 
 * Get the user master password and use it to unlock the store.
 */
void VaultController::unlockEncryptedStore()
{
    SafeBuffer *masterPassword = new SafeBuffer(MASTER_PASSWORD_MAX_SIZE);

    if (this->terminal.readString(TXT_ENTER_MASTER_PASSWORD, masterPassword, TXT_PASSWORD_MASK, TERMINAL_FIRST_CANVAS_LINE + 2, 2))
    {
        this->encryptedStore.unlock(masterPassword);
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
    this->terminal.print("Select position: ", TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 2);

    byte selectedIndex = this->terminal.waitKeySelection('a', 'a' + ENCRYPTED_STORE_MAX_ENTRIES);

    if (selectedIndex == TERMINAL_OPERATION_ABORTED)
    {
        return;
    }

    this->terminal.clearCanvas();

    SafeBuffer *label = new SafeBuffer(ENCRYPTED_STORE_LABEL_SIZE);
    SafeBuffer *password = new SafeBuffer(ENCRYPTED_STORE_DATA_SIZE);

    this->terminal.readString("Enter label: ", label, 0, TERMINAL_FIRST_CANVAS_LINE + 2, 2);
    this->terminal.readString("Enter password: ", password, '*', TERMINAL_FIRST_CANVAS_LINE + 3, 2);

    this->terminal.printStatusMessage(" Enctrypting......");
    this->encryptedStore.set(selectedIndex, password, label);

    delete label;
    delete password;

    this->displayPasswordSelectionMenu();
}

/**
 * Wipe a password.
 * 
 * Asks the user the position to wipe.
 */
void VaultController::wipePassword()
{
    this->terminal.print("Select position to wipe: ", TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 2);

    byte selectedIndex = this->terminal.waitKeySelection('a', 'a' + ENCRYPTED_STORE_MAX_ENTRIES);

    if (selectedIndex == TERMINAL_OPERATION_ABORTED)
    {
        return;
    }

    this->encryptedStore.wipe(selectedIndex);

    this->displayPasswordSelectionMenu();
}

void VaultController::lockStore()
{
    this->encryptedStore.lock();
    memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
}

void VaultController::selectPassword(byte index)
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    if (index >= ENCRYPTED_STORE_MAX_ENTRIES)
    {
        return;
    }

    this->encryptedStore.getLabel(index, label);

    if (strlen(label) == 0)
    {
        this->terminal.printStatusMessage(" No password stored here.");
        delay(2000);
        this->displayPasswordSelectionMenu();
        return;
    }

    char message[TERMINAL_WIDTH];
    sprintf(message, " Selected: %s", label);
    this->terminal.printStatusMessage(message);

    selectedPasswordIndex = index;
    this->displayPasswordActionMenu();
}

void VaultController::displayPasswordSelectionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    this->terminal.clearScreen();

    for (byte menuPosition = 0; menuPosition < ENCRYPTED_STORE_MAX_ENTRIES; menuPosition++)
    {
        this->encryptedStore.getLabel(menuPosition, label);

        if (strlen(label) == 0)
        {
            strcpy(label, "---");
        }

        this->terminal.printMenuEntry(menuPosition, label);
    }

    this->terminal.clearHotkeys();
    this->terminal.addHotkey('a', makeFunctor((Functor0 *)0, *this, &VaultController::addPassword));
    this->terminal.addHotkey('w', makeFunctor((Functor0 *)0, *this, &VaultController::wipePassword));
    this->terminal.addHotkey('l', makeFunctor((Functor0 *)0, *this, &VaultController::lockStore));
    this->terminal.setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::selectPassword));
    this->terminal.printStatusMessage(" ALT+A Add  |  ALT+W Wipe  |  ALT+L Lock  |  ALT+Q Reset");
}

void VaultController::actOnPassword(byte action)
{
    switch (action)
    {
    case 0:
        this->encryptedStore.get(selectedPasswordIndex, clipboard);
        break;
    case 1:
        char buffer[TERMINAL_WIDTH];

        this->terminal.clearCanvas();
        this->terminal.readString("Enter tokens positions: ", buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 1, 2);
        this->encryptedStore.getTokens(selectedPasswordIndex, buffer, clipboard);
        break;
    default:
        return;
    }

    this->terminal.clearCanvas();

#ifdef OPTION_BELLS_AND_WHISTLES
    // Just for a show, the decryption is already done.
    this->terminal.printStatusProgress("Fetch record", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 2, 2, 30);
    this->terminal.printStatusProgress("Decrypt", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 3, 2, 30);
    this->terminal.printStatusProgress("Copy to clipboard", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 4, 2, 30);
#endif
    this->terminal.print("Ready.", TERMINAL_FIRST_CANVAS_LINE + 5, 2);

    while (strlen(clipboard) > 0)
    {
        if (digitalRead(BUTTON_A_SENSE) == LOW)
        {
            Keyboard.print(clipboard);
            memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
        }

        loop();
    }

    this->displayPasswordSelectionMenu();
}

void VaultController::displayPasswordActionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    this->terminal.clearCanvas();
    this->terminal.printMenuEntry(0, "Copy to clipboard");
    this->terminal.printMenuEntry(1, "Partial copy to clipboard");

    this->terminal.clearHotkeys();
    this->terminal.setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::actOnPassword));
}

void VaultController::resetTerminal()
{
    this->encryptedStore.lock();
    memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);

    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    delay(500);
    this->terminal.init(&Serial);
}

void VaultController::begin()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // Wait for someone to connect
    }
    delay(500);
    this->terminal.init(&Serial);

    memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);

    this->terminal.setResetCallback(makeFunctor((Functor0 *)0, *this, &VaultController::resetTerminal));
}

void VaultController::loop()
{
    this->terminal.setClpIndicator(strlen(clipboard) > 0);
    this->terminal.setLclIndicator(encryptedStore.isLocked());

    if (encryptedStore.isLocked())
    {
        this->terminal.clearScreen();
        this->terminal.printStatusMessage(" Locked.");
        this->unlockEncryptedStore();
        this->displayPasswordSelectionMenu();

        return;
    }

    this->terminal.loop();
}

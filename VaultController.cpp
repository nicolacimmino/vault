
#include "VaultController.h"

VaultController::VaultController()
{
    this->clipboard = new SafeBuffer(ENCRYPTED_STORE_DATA_SIZE);
}

VaultController::~VaultController()
{
    delete this->clipboard;
}

/**
 * Unlock the store.
 * 
 * Get the user master password and use it to unlock the store.
 */
void VaultController::unlockEncryptedStore()
{
    SafeBuffer *masterPassword = new SafeBuffer(ENCRYPTED_STORE_MASTER_PASSWORD_MAX_SIZE);

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

    if (selectedIndex != TERMINAL_OPERATION_ABORTED)
    {
        this->terminal.clearCanvas();

        SafeBuffer *label = new SafeBuffer(ENCRYPTED_STORE_LABEL_SIZE);
        SafeBuffer *password = new SafeBuffer(ENCRYPTED_STORE_DATA_SIZE);

        this->terminal.readString("Enter label: ", label, 0, TERMINAL_FIRST_CANVAS_LINE + 2, 2);
        this->terminal.readString("Enter password: ", password, '*', TERMINAL_FIRST_CANVAS_LINE + 3, 2);

        this->terminal.printStatusMessage(" Enctrypting......");
        this->encryptedStore.set(selectedIndex, password, label);

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
    this->terminal.print("Select position to wipe: ", TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 2);

    byte selectedIndex = this->terminal.waitKeySelection('a', 'a' + ENCRYPTED_STORE_MAX_ENTRIES);

    if (selectedIndex != TERMINAL_OPERATION_ABORTED)
    {
        this->encryptedStore.wipe(selectedIndex);
    }

    this->displayPasswordSelectionMenu();
}

/**
 * Lock the store and wipe the clipboard.
 */
void VaultController::lockEnctryptedStore()
{
    this->encryptedStore.lock();
    this->clipboard->wipe();
}

/**
 * Select password to perform actions on.
 */
void VaultController::selectPassword(byte index)
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    if (index >= ENCRYPTED_STORE_MAX_ENTRIES || this->encryptedStore.isPositionFree(index))
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
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    this->terminal.clearScreen();

    for (byte menuPosition = 0; menuPosition < ENCRYPTED_STORE_MAX_ENTRIES; menuPosition++)
    {
        if (!this->encryptedStore.getLabel(menuPosition, label))
        {
            strcpy(label, "---");
        }

        this->terminal.printMenuEntry(menuPosition, label);
    }

    this->terminal.clearHotkeys();
    this->terminal.addHotkey('a', makeFunctor((Functor0 *)0, *this, &VaultController::addPassword));
    this->terminal.addHotkey('w', makeFunctor((Functor0 *)0, *this, &VaultController::wipePassword));
    this->terminal.addHotkey('l', makeFunctor((Functor0 *)0, *this, &VaultController::lockEnctryptedStore));
    this->terminal.addHotkey('i', makeFunctor((Functor0 *)0, *this, &VaultController::showInfoScreen));
    this->terminal.addHotkey('b', makeFunctor((Functor0 *)0, *this, &VaultController::backup));
    this->terminal.printStatusMessage(" ALT+A Add  |  ALT+W Wipe  |  ALT+L Lock  |  ALT+I Info  |  ALT+B Backup");

    this->terminal.setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::selectPassword));
}

void VaultController::showInfoScreen()
{
    this->terminal.clearCanvas();
    this->terminal.print("FW Fingerprint:", TERMINAL_FIRST_CANVAS_LINE + 2, 2);
    this->terminal.printBufferHex(this->encryptedStore.getFirmwareFingerprint(), ENCRYPTED_STORE_FW_FINGERPRINT_SIZE);
}

void VaultController::retrievePassword(byte action)
{
    switch (action)
    {
    case 0:
    {
        this->encryptedStore.get(selectedPasswordIndex, clipboard);
        break;
    }
    case 1:
    {
        SafeBuffer *buffer = new SafeBuffer(TERMINAL_WIDTH);

        this->terminal.readString("Enter tokens positions: ", buffer, 0, TERMINAL_FIRST_CANVAS_LINE + 4, TERMINAL_RIGHT_HALF_FIRST_COLUMN);
        this->encryptedStore.getTokens(selectedPasswordIndex, buffer->getBuffer(), clipboard);

        delete buffer;
        break;
    }
    case 2:
    {
        this->displayPasswordSelectionMenu();
        return;
    }
    default:
        return;
    }

#ifdef OPTION_BELLS_AND_WHISTLES
    // Just for a show, the decryption is already done.
    this->terminal.printStatusProgress("Fetch record", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 6, TERMINAL_RIGHT_HALF_FIRST_COLUMN, 30);
    this->terminal.printStatusProgress("Decrypt", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 7, TERMINAL_RIGHT_HALF_FIRST_COLUMN, 30);
    this->terminal.printStatusProgress("Copy to clipboard", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 8, TERMINAL_RIGHT_HALF_FIRST_COLUMN, 30);
#endif
    this->terminal.print("Ready.", TERMINAL_FIRST_CANVAS_LINE + 9, TERMINAL_RIGHT_HALF_FIRST_COLUMN);

    while (clipboard->strlen() > 0)
    {
        if (digitalRead(BUTTON_A_SENSE) == LOW)
        {
            Keyboard.print(this->clipboard->getBuffer());
            this->clipboard->wipe();
        }

        loop();
    }

    this->displayPasswordSelectionMenu();
}

void VaultController::displayPasswordActionMenu()
{
    this->terminal.printMenuEntry(0, "Copy to clipboard", true);
    this->terminal.printMenuEntry(1, "Partial copy to clipboard", true);
    this->terminal.printMenuEntry(2, "Back", true);

    this->terminal.setMenuCallback(makeFunctor((Functor1<byte> *)0, *this, &VaultController::retrievePassword));
}

void VaultController::resetTerminal()
{
    Serial.begin(9600);

    this->lockEnctryptedStore();

    this->notificationLight.begin();
    this->notificationLight.setBreathe(true);
    this->notificationLight.setBrightness(50);
    this->notificationLight.setColor(CRGB::Red);

    while (!Serial)
    {
        this->notificationLight.loop();
    }
    delay(500);

    this->terminal.init(&Serial);

    this->notificationLight.setBreathe(false);

    this->terminal.setResetCallback(makeFunctor((Functor0 *)0, *this, &VaultController::resetTerminal));
}

void VaultController::backup()
{
    while (digitalRead(BUTTON_A_SENSE) == HIGH)
    {
        loop();
    }

    SafeBuffer *asciiPrint = new SafeBuffer(16);

    for (uint16_t address = 0; address < ENCRYPTED_STORE_EEPROM_SIZE; address++)
    {
        byte value = EEPROM.read(address);
        Keyboard.print(value >> 4, HEX);
        Keyboard.print(value & 4, HEX);
        asciiPrint->setChar(address % 16, value > 31 && value < 127 ? (char)value : '.');

        if (address % 16 == 15)
        {
            Keyboard.print("\t");
            Keyboard.print(asciiPrint->getBuffer());
            Keyboard.print("\n");
        }
        else
        {
            Keyboard.print(".");
        }
    }

    delete asciiPrint;
}

void VaultController::loop()
{
    this->terminal.setClpIndicator(this->clipboard->strlen() > 0);
    this->terminal.setLclIndicator(encryptedStore.isLocked());

    if (encryptedStore.isLocked())
    {
        this->notificationLight.setColor(CRGB::Red);
        this->notificationLight.loop();
        this->terminal.clearScreen();
        this->terminal.printStatusMessage(" Locked.");
        this->unlockEncryptedStore();
        this->displayPasswordSelectionMenu();
        this->terminal.setKeyFingerprint(encryptedStore.getKeyFingerprint());

        return;
    }

    if (this->clipboard->strlen() > 0)
    {
        this->notificationLight.setColor(CRGB::Yellow);
    }
    else
    {
        this->notificationLight.setColor(CRGB::Green);
    }

    this->notificationLight.loop();
    this->terminal.loop();
}

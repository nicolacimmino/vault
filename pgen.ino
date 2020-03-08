
//  Copyright (C) 2020 Nicola Cimmino
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "Terminal.h"
#include "EncryptedStore.h"
#include "sha256.h"
#include <Keyboard.h>
#include "options.h"

#define BUTTON_A_COMMON 7
#define BUTTON_A_SENSE 8
#define MASTER_PASSWORD_MAX_SIZE 64

Terminal terminal;
EncryptedStore encryptedStore;
char clipboard[ENCRYPTED_STORE_DATA_SIZE];
byte selectedPasswordIndex;

void unlockEncryptedStore()
{
    char masterPassword[MASTER_PASSWORD_MAX_SIZE];
    memset(masterPassword, 0, MASTER_PASSWORD_MAX_SIZE);
    terminal.readString("Enter master password: ", masterPassword, MASTER_PASSWORD_MAX_SIZE, '*', TERMINAL_FIRST_CANVAS_LINE + 2, 2);
    encryptedStore.unlock(masterPassword);
    memset(masterPassword, 0, MASTER_PASSWORD_MAX_SIZE);
}

void addPassword()
{
    char selection[4];
    terminal.readString("Select position: ", selection, 4, 0, TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 2);
    byte selectedIndex = selection[0] - 'a';

    terminal.clearCanvas();
        
    char label[ENCRYPTED_STORE_LABEL_SIZE];
    char password[ENCRYPTED_STORE_DATA_SIZE];

    terminal.readString("Enter label: ", label, ENCRYPTED_STORE_LABEL_SIZE, 0, TERMINAL_FIRST_CANVAS_LINE + 2, 2);
    terminal.readString("Enter password: ", password, ENCRYPTED_STORE_DATA_SIZE, '*', TERMINAL_FIRST_CANVAS_LINE + 3, 2);

    terminal.printStatusMessage(" Enctrypting......");
    encryptedStore.set(selectedIndex, password, label);
    displayPasswordSelectionMenu();
}

void wipePassword()
{
    char selection[4];
    terminal.readString("Select position to wipe: ", selection, 4, 0, TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 2);

    encryptedStore.wipe(selection[0] - 'a');

    displayPasswordSelectionMenu();
}

void lockStore()
{
    encryptedStore.lock();
    memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
}

void selectPassword(byte index)
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    if (index >= ENCRYPTED_STORE_MAX_ENTRIES)
    {
        return;
    }

    encryptedStore.getLabel(index, label);

    if (strlen(label) == 0)
    {
        terminal.printStatusMessage(" No password stored here.");
        delay(2000);
        displayPasswordSelectionMenu();
        return;
    }

    char message[TERMINAL_WIDTH];
    sprintf(message, " Selected: %s", label);
    terminal.printStatusMessage(message);

    selectedPasswordIndex = index;
    displayPasswordActionMenu();
}

void displayPasswordSelectionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    terminal.clearScreen();

    for (byte menuPosition = 0; menuPosition < ENCRYPTED_STORE_MAX_ENTRIES; menuPosition++)
    {
        encryptedStore.getLabel(menuPosition, label);

        if (strlen(label) == 0)
        {
            strcpy(label, "---");
        }

        terminal.printMenuEntry(menuPosition, label);
    }

    terminal.clearHotkeys();
    terminal.addHotkey('a', addPassword);
    terminal.addHotkey('w', wipePassword);
    terminal.addHotkey('l', lockStore);
    terminal.setMenuCallback(selectPassword);
    terminal.printStatusMessage(" ALT+A Add  |  ALT+W Wipe  |  ALT+L Lock  |  ALT+Q Reset");
}

void actOnPassword(byte action)
{
    switch (action)
    {
    case 0:
        encryptedStore.get(selectedPasswordIndex, clipboard);
        break;
    case 1:
        char buffer[TERMINAL_WIDTH];

        terminal.clearCanvas();
        terminal.readString("Enter tokens positions: ", buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 1, 2);
        encryptedStore.getTokens(selectedPasswordIndex, buffer, clipboard);
        break;
    default:
        return;
    }

    terminal.clearCanvas();

#ifdef OPTION_BELLS_AND_WHISTLES
    // Just for a show, the decryption is already done.
    terminal.printStatusProgress("Fetch record", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 2, 2, 30);
    terminal.printStatusProgress("Decrypt", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 3, 2, 30);
    terminal.printStatusProgress("Copy to clipboard", 600, "[OK]", TERMINAL_FIRST_CANVAS_LINE + 4, 2, 30);
#endif
    terminal.print("Ready.", TERMINAL_FIRST_CANVAS_LINE + 5, 2);

    while (strlen(clipboard) > 0)
    {
        if (digitalRead(BUTTON_A_SENSE) == LOW)
        {
            Keyboard.print(clipboard);
            memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
        }

        loop();
    }

    displayPasswordSelectionMenu();
}

void displayPasswordActionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    terminal.clearCanvas();
    terminal.printMenuEntry(0, "Copy to clipboard");
    terminal.printMenuEntry(1, "Partial copy to clipboard");

    terminal.clearHotkeys();
    terminal.setMenuCallback(actOnPassword);
}

void resetTerminal()
{
    encryptedStore.lock();
    memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);

    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    delay(500);
    terminal.init(&Serial);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    delay(500);
    terminal.init(&Serial);

    memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);

    pinMode(BUTTON_A_COMMON, OUTPUT);
    digitalWrite(BUTTON_A_COMMON, LOW);
    pinMode(BUTTON_A_SENSE, INPUT_PULLUP);

    Keyboard.begin();
    terminal.setResetCallback(resetTerminal);
}

void loop()
{
    terminal.setClpIndicator(strlen(clipboard) > 0);
    terminal.setLclIndicator(encryptedStore.isLocked());

    if (encryptedStore.isLocked())
    {
        terminal.clearScreen();
        terminal.printStatusMessage(" Locked.");
        unlockEncryptedStore();
        displayPasswordSelectionMenu();

        return;
    }

    terminal.loop();
}

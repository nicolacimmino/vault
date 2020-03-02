
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
#include "EncryptedStoreKey.h"
#include "sha256.h"

Terminal terminal;
EncryptedStore encryptedStore;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    delay(500);
    terminal.init(&Serial);
}

#define MASTER_PASSWORD_MAX_SIZE 64

void unlockEncryptedStore()
{
    char masterPassword[MASTER_PASSWORD_MAX_SIZE];
    memset(masterPassword, 0, MASTER_PASSWORD_MAX_SIZE);
    terminal.print("Enter master password: ", TERMINAL_FIRST_CANVAS_LINE + 2, 1);
    terminal.readString(masterPassword, MASTER_PASSWORD_MAX_SIZE, '*');
    encryptedStore.unlock(masterPassword);
    memset(masterPassword, 0, MASTER_PASSWORD_MAX_SIZE);
}

void addPassword()
{
    terminal.clearCanvas();

    byte firstFreeSlot = encryptedStore.getFirstFreeSlot();

    if (firstFreeSlot == ENCRYPTED_STORE_FULL)
    {
        terminal.printStatusMessage("Store full! Delete something first.");
        delay(2000);
        return;
    }

    char label[ENCRYPTED_STORE_LABEL_SIZE];
    char password[ENCRYPTED_STORE_DATA_SIZE];

    terminal.print("Enter label: ", TERMINAL_FIRST_CANVAS_LINE + 2, 1);
    terminal.readString(label, ENCRYPTED_STORE_LABEL_SIZE);
    terminal.print("Enter password: ", TERMINAL_FIRST_CANVAS_LINE + 3, 1);
    terminal.readString(password, ENCRYPTED_STORE_DATA_SIZE, '*');

    terminal.printStatusMessage("Enctrypting......");
    encryptedStore.set(firstFreeSlot, password, label);
}

void wipePassword()
{
    terminal.printStatusMessage("Select position to wipe: ");

    char selection[1];
    terminal.readString(selection, 1);

    encryptedStore.wipe(selection[0] - 'a');

    displayPasswordSelectionMenu();
}

void lockStore()
{
    encryptedStore.lock();
}

void displayPasswordSelectionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    terminal.clearCanvas();

    for (byte menuPosition = 0; menuPosition < ENCRYPTED_STORE_MAX_ENTRIES; menuPosition++)
    {
        encryptedStore.getLabel(menuPosition, label);

        if (strlen(label) == 0)
        {
            terminal.printMenuEntry(menuPosition, "[free]");
            continue;
        }

        terminal.printMenuEntry(menuPosition, label);
    }

    terminal.setShortcut(0, 'A', 1);
    terminal.setShortcut(1, 'W', 2);
    terminal.setShortcut(2, 'L', 3);
    terminal.printStatusMessage("A Add  -  W Wipe  -  L Lock");
    byte selection = terminal.waitMenuSelection();
    if (selection == 1)
    {
        addPassword();
    }

    if (selection == 2)
    {
        wipePassword();
    }

    if (selection == 3)
    {
        lockStore();
    }

    terminal.clearCanvas();
}

void loop()
{
    terminal.clearScreen();
    terminal.printBanner();

    if (encryptedStore.isLocked())
    {
        terminal.printStatusMessage("Locked.");
        unlockEncryptedStore();
    }

    displayPasswordSelectionMenu();
}
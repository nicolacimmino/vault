
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

#define BUTTON_A_COMMON 7
#define BUTTON_A_SENSE 8
#define MASTER_PASSWORD_MAX_SIZE 64

Terminal terminal;
EncryptedStore encryptedStore;
char clipboard[ENCRYPTED_STORE_DATA_SIZE];
byte selectedPasswordIndex;

/**
 * See https://playground.arduino.cc/Code/AvailableMemory/
 **/
int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void printHeader()
{
    char headerMessage[TERMINAL_WIDTH];
    memset(headerMessage, 0, TERMINAL_WIDTH);

    sprintf(headerMessage, "Vault V0.1 - %d bytes free %s %s ",
            freeRam(),
            (strlen(clipboard) > 0) ? "[CLP]" : "",
            (encryptedStore.isLocked() ? "[LCK]" : "[ULK]"));

    terminal.printHeaderMessage(headerMessage);
}

void clearScreen()
{
    terminal.clearScreen();
    printHeader();
    terminal.printBanner();
    terminal.resetInactivityTimer();
}

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
    displayPasswordSelectionMenu();
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

void selectPassword(byte index)
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    encryptedStore.getLabel(index, label);

    if (strlen(label) == 0)
    {
        terminal.printStatusMessage("No password stored here.");
        delay(2000);
        displayPasswordSelectionMenu();
        return;
    }

    char message[TERMINAL_WIDTH];
    sprintf(message, "Selected: %s", label);
    terminal.printStatusMessage(message);

    selectedPasswordIndex = index;
    displayPasswordActionMenu();
}

void displayPasswordSelectionMenu()
{
    char label[ENCRYPTED_STORE_LABEL_SIZE];

    clearScreen();

    for (byte menuPosition = 0; menuPosition < ENCRYPTED_STORE_MAX_ENTRIES; menuPosition++)
    {
        encryptedStore.getLabel(menuPosition, label);

        if (strlen(label) == 0)
        {
            terminal.printMenuEntry(menuPosition, "---");
            continue;
        }

        terminal.printMenuEntry(menuPosition, label);
    }

    terminal.clearHotkeys();
    terminal.addHotkey('a', addPassword);
    terminal.addHotkey('w', wipePassword);
    terminal.addHotkey('l', lockStore);
    terminal.setMenuCallback(selectPassword);
    terminal.printStatusMessage("ALT+A Add  -  ALT+W Wipe  -  ALT+L Lock");
}

void actOnPassword(byte action)
{
    if (action == 0)
    {
        encryptedStore.get(selectedPasswordIndex, clipboard);
        terminal.clearCanvas();
        terminal.printStatusMessage("Password in clipboard.");
    }

    if (action == 1)
    {
        char buffer[TERMINAL_WIDTH];
        char password[ENCRYPTED_STORE_DATA_SIZE];
        encryptedStore.get(selectedPasswordIndex, password);
        memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);

        terminal.clearCanvas();
        terminal.print("Enter tokens positions: ", TERMINAL_FIRST_CANVAS_LINE + 2, 1);
        terminal.readString(buffer, TERMINAL_WIDTH);
        byte ix = 0;
        char *token = strtok(buffer, ",");
        while (token != NULL)
        {
            int tokenIndex = atoi(token) - 1;
            clipboard[ix] = password[tokenIndex];
            token = strtok(NULL, ",");
            ix++;
        }
        terminal.printStatusMessage("Password tokens in clipboard.");
    }
    delay(2000);
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
    if (millis() % 300 == 0)
    {
        printHeader();
    }

    if (encryptedStore.isLocked())
    {
        clearScreen();
        terminal.printStatusMessage("Locked.");
        unlockEncryptedStore();        
        displayPasswordSelectionMenu();

        return;
    }

    if (digitalRead(BUTTON_A_SENSE) == LOW && strlen(clipboard) > 0)
    {
        Keyboard.print(clipboard);
        memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
    }

    terminal.loop();
}

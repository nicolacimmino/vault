
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

    sprintf(headerMessage, " Vault V0.1 - %d bytes free %s %s ",
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
    terminal.readString("Enter master password: ", masterPassword, MASTER_PASSWORD_MAX_SIZE, '*', TERMINAL_FIRST_CANVAS_LINE + 2, 2);
    encryptedStore.unlock(masterPassword);
    memset(masterPassword, 0, MASTER_PASSWORD_MAX_SIZE);
}

void addPassword()
{
    terminal.clearCanvas();

    byte firstFreeSlot = encryptedStore.getFirstFreeSlot();

    if (firstFreeSlot == ENCRYPTED_STORE_FULL)
    {
        terminal.printStatusMessage(" Store full! Delete something first.");
        delay(2000);
        return;
    }

    char label[ENCRYPTED_STORE_LABEL_SIZE];
    char password[ENCRYPTED_STORE_DATA_SIZE];

    terminal.readString("Enter label: ", label, ENCRYPTED_STORE_LABEL_SIZE, 0, TERMINAL_FIRST_CANVAS_LINE + 2, 2);
    terminal.readString("Enter password: ", password, ENCRYPTED_STORE_DATA_SIZE, '*', TERMINAL_FIRST_CANVAS_LINE + 3, 2);

    terminal.printStatusMessage(" Enctrypting......");
    encryptedStore.set(firstFreeSlot, password, label);
    displayPasswordSelectionMenu();
}

void wipePassword()
{
    char selection[1];
    terminal.readString("Select position to wipe: ", selection, 1, 0, TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 1);

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
    terminal.printStatusMessage(" ALT+A Add  |  ALT+W Wipe  |  ALT+L Lock  |  ALT+Q Reset");
}

void actOnPassword(byte action)
{
    if (action == 0)
    {
        encryptedStore.get(selectedPasswordIndex, clipboard);
    }

    if (action == 1)
    {
        char buffer[TERMINAL_WIDTH];
        char password[ENCRYPTED_STORE_DATA_SIZE];
        encryptedStore.get(selectedPasswordIndex, password);
        memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);

        terminal.clearCanvas();
        terminal.readString("Enter tokens positions: ", buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 1, 2);
        byte ix = 0;
        char *token = strtok(buffer, ",");
        while (token != NULL)
        {
            int tokenIndex = atoi(token) - 1;
            clipboard[ix] = password[tokenIndex];
            token = strtok(NULL, ",");
            ix++;
        }
    }

    // Just for a show, the decryption is already done.
    terminal.clearCanvas();
    terminal.print(VT_FOREGROUND_YELLOW " Fetch record", TERMINAL_FIRST_CANVAS_LINE + 2, 1);
    delay(600);
    terminal.print("           " VT_FOREGROUND_GREEN "[OK]");
    terminal.print(VT_FOREGROUND_YELLOW " Decrypt password", TERMINAL_FIRST_CANVAS_LINE + 3, 1);
    delay(600);
    terminal.print("       " VT_FOREGROUND_GREEN "[OK]");
    terminal.print(VT_FOREGROUND_YELLOW " Password in clipboard  " VT_FOREGROUND_GREEN "[OK]", TERMINAL_FIRST_CANVAS_LINE + 4, 1);
    terminal.print(VT_FOREGROUND_YELLOW " Ready.  ", TERMINAL_FIRST_CANVAS_LINE + 5, 1);

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
    if (millis() % 300 == 0)
    {
        printHeader();
    }

    if (encryptedStore.isLocked())
    {
        clearScreen();
        terminal.printStatusMessage(" Locked.");
        unlockEncryptedStore();
        displayPasswordSelectionMenu();

        return;
    }

    terminal.loop();
}

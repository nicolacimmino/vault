
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

void initStoreWithMasterPassword()
{
    char masterPassword[MASTER_PASSWORD_MAX_SIZE];
    memset(masterPassword, 0, MASTER_PASSWORD_MAX_SIZE);
    terminal.printMasterPasswordPrompt();
    terminal.readMasterPassword(masterPassword, MASTER_PASSWORD_MAX_SIZE);
    encryptedStore.init(masterPassword);
    memset(masterPassword, 0, MASTER_PASSWORD_MAX_SIZE);
}

void loop()
{
    terminal.clearScreen();
    terminal.printBanner();

    initStoreWithMasterPassword();

    char text[32];
    memset(text, 0, 32);

    terminal.printStatusMessage("Decrypting....");
    //encryptedStore.set(0, text);
    delay(2000);
    terminal.printStatusMessage("Done.");
    // //encryptedStore.set(1, text);

    encryptedStore.get(0, text);

    VT100.setCursor(8, 1);
    VT100.clearLineAfter();
    VT100.setCursor(9, 1);
    Serial.print("Password:");
    Serial.println(text);
    VT100.clearLineAfter();

    delay(2000);
}
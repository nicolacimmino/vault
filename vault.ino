
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

#include "hardware.h"
#include "options.h"
#include "VaultController.h"

/**
 * We couuld use functors for the menus callbacks, but since VaultController is 
 * a singleton we just lazily do with these globals passed as function pointers
 * to the terminal.
 */

void invoke_vaultController_addPassword()
{
    VaultController::getInstance()->addPassword();
}

void invoke_vaultController_wipePassword()
{
    VaultController::getInstance()->wipePassword();
}

void invoke_vaultController_lockStore()
{
    VaultController::getInstance()->lockStore();
}

void invoke_vaultController_selectPassword(byte index)
{
    VaultController::getInstance()->selectPassword(index);
}

void invoke_vaultController_actOnPassword(byte action)
{
    VaultController::getInstance()->actOnPassword(action);
}

void invoke_vaultController_resetTerminal()
{
    VaultController::getInstance()->resetTerminal();
}

void setup()
{
    pinMode(BUTTON_A_COMMON, OUTPUT);
    digitalWrite(BUTTON_A_COMMON, LOW);
    pinMode(BUTTON_A_SENSE, INPUT_PULLUP);

    VaultController::getInstance()->begin();

    Keyboard.begin();
}

void loop()
{
    VaultController::getInstance()->loop();
}

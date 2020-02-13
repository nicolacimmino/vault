
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

#include <VT100.h>
#include "messages.h";

void setup()
{
    Serial.begin(9600);
    VT100.begin(Serial);
    VT100.cursorSave();
}

char buffer[255];

void printMessage(uint8_t messageId)
{
    strcpy_P(buffer, (char *)pgm_read_word(&(messages[messageId])));
    Serial.print(buffer);
}

void loop()
{
    if (Serial.available())
    {
        char c = Serial.read();

        if (c == '0')
        {
            VT100.setBackgroundColor(VT_BLACK);
            VT100.clearScreen();

            VT100.setBackgroundColor(VT_BLACK);
            VT100.setTextColor(VT_GREEN);
            VT100.setCursor(1, 1);

            printMessage(0);

            VT100.setCursor(9, 1);

            printMessage(1);
        }

        if (c == '1')
        {
            VT100.setCursor(9, 1);
            printMessage(2);
        }
    }
}
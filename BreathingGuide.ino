
//  Copyright (C) 2019 Nicola Cimmino
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
#include "Accelerometer.h"
#include "LEDBarController.h"
#include "ControlButton.h"
#include "ModeExecutor.h"
#include "FloodLight.h"
#include "FourByFourModeExecutor.h"
#include "RainbowModeExecutor.h"

Accelerometer accelerometer;
LEDBarController ledBarController;
ControlButton controlButton;
FloodLight floodLight;

uint8_t mode = 0;
ModeExecutor *modeExecutors[] = {new FourByFourModeExecutor(), new RainbowModeExecutor()};

void onClick()
{
    modeExecutors[mode]->exitMode();
    mode = (mode + 1) % 2;
    modeExecutors[mode]->enterMode();
}

void onLongPress()
{
}

void setup()
{

    Serial.begin(115200);

    pinMode(PIN_BTN_C, OUTPUT);
    digitalWrite(PIN_BTN_C, LOW);

    controlButton.begin(PIN_BTN_S, &onClick, &onLongPress);

    uint8_t ledPins[] = {PIN_LED_G0, PIN_LED_G1, PIN_LED_G2, PIN_LED_G3};
    ledBarController.begin(ledPins);

    floodLight.begin();
    floodLight.setBrightness(digitalRead(PIN_BTN_S) == LOW ? 15 : 255);

    accelerometer.begin(PIN_ACC_X, PIN_ACC_Y, PIN_ACC_Z);

    for (int ix = 0; ix < 2; ix++)
    {
        modeExecutors[ix]->begin(&floodLight, &ledBarController, &accelerometer);
    }
    modeExecutors[0]->enterMode();
}

void loop()
{
    accelerometer.loop();
    controlButton.loop();

    for (int ix = 0; ix < 2; ix++)
    {
        modeExecutors[ix]->loop();
    }

    ledBarController.loop();
    floodLight.loop();
}

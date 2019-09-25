
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
#include "FloodLight.h"

#include <FastLED.h>

Accelerometer accelerometer;
LEDBarController ledBarController;
ControlButton controlButton;
FloodLight floodLight;

uint8_t mode = 0;
uint32_t modeChangeTime;

void onClick()
{
    mode = (mode + 1) % 2;
    modeChangeTime = millis();
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
}

void loop()
{

    //  Serial.println(accelerometer.getX());
    // Serial.println(accelerometer.getY());
    // Serial.println(accelerometer.getZ());
    // Serial.println("--");

    ledBarController.loop();
    controlButton.loop();
    floodLight.loop();

    if (millis() - modeChangeTime < 1000)
    {
        floodLight.setColor(CRGB::Red, mode == 0 ? CRGB::Blue : CRGB::Green);
        floodLight.setFade(255);
        return;
    }

    if (mode == 1)
    {
        ledBarController.setDim();
        ledBarController.showBar(0);

        floodLight.setColor(CRGB::CRGB(accelerometer.getX() + 127, accelerometer.getY() + 127, accelerometer.getZ() + 127));
        floodLight.setFade(255);
    }

    if (mode == 0)
    {
        uint32_t timePhase = (millis() / 1000) % 5;

        ledBarController.setDim();
        if (accelerometer.getY() < -100)
        {
            ledBarController.setFullBrightness();
        }

        ledBarController.showBar(timePhase);

        float val = (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;

        floodLight.setColor(CRGB::DarkGreen);
        floodLight.setFade(val);
    }
}

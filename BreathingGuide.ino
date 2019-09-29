
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
#include "BatteryMonitor.h"

#define EXECUTORS_COUNT 5
#include "ModeExecutor.h"
#include "SquareBreathModeExecutor.h"
#include "AlternateNostrilBreathModeExecutor.h"
#include "RainbowModeExecutor.h"
#include "PomodoroModeExecutor.h"
#include "NightlightModeExecutor.h"
#include "ModesChanger.h"

#include <avr/sleep.h>

Accelerometer accelerometer;
LEDBarController ledBarController;
ControlButton controlButton;
FloodLight floodLight;
BatteryMonitor BatteryMonitor;

uint8_t mode = 0;
ModeExecutor *modeExecutors[] = {new SquareBreathModeExecutor(), new AlternateNostrilBreathModeExecutor(), new RainbowModeExecutor(), new PomodoroModeExecutor(), new NighlightModeExecutor()};

ModesChanger modesChanger;

void onClick()
{
    if (modesChanger.isActive())
    {
        modesChanger.onClick();
    }
}

void onLongPress()
{
    modesChanger.onLongPress();
}

void onTiltX(bool positive)
{
    for (int ix = 0; ix < EXECUTORS_COUNT; ix++)
    {
        modeExecutors[ix]->onTiltX(positive);
    }
}

void onShake()
{
    for (int ix = 0; ix < EXECUTORS_COUNT; ix++)
    {
        modeExecutors[ix]->onShake();
    }
}

void onLowBattery()
{
    floodLight.override(300, CRGB::Red, 0);
}

void onBatteryCritical()
{
    ledBarController.showBar(0);
    floodLight.animateSync(CRGB::Red, CRGB::Black, 200, 10);
    floodLight.shutdown();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
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
    floodLight.animateSync(CRGB::Green, CRGB::Yellow, 200, 10);

    accelerometer.begin(PIN_ACC_X, PIN_ACC_Y, PIN_ACC_Z, onTiltX, NULL, NULL, onShake);

    for (int ix = 0; ix < EXECUTORS_COUNT; ix++)
    {
        modeExecutors[ix]->begin(&floodLight, &ledBarController, &accelerometer);
    }
    modeExecutors[0]->enterMode();

    BatteryMonitor.begin(onLowBattery, onBatteryCritical);

    modesChanger.begin(modeExecutors, EXECUTORS_COUNT, &floodLight);
}

void loop()
{
    accelerometer.loop();
    controlButton.loop();
    modesChanger.loop();

    if (!modesChanger.isActive())
    {
        for (int ix = 0; ix < EXECUTORS_COUNT; ix++)
        {
            modeExecutors[ix]->loop();
        }
    }

    ledBarController.loop();
    BatteryMonitor.loop();
    floodLight.loop();    
}

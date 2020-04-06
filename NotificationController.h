
#ifndef __NOTIFICATION_CONTROLLER_H__
#define __NOTIFICATION_CONTROLLER_H__

#include <Arduino.h>
#include "hardware.h"

#ifdef NEOPIXEL_NOTIFICATION
#include <FastLED.h>
#endif

class NotificationController
{
private:
    bool ledRed = false;    
    bool ledYellowBlink = false;
    bool ledGreen = false;
#ifdef NEOPIXEL_NOTIFICATION
    CRGB led[1];
#endif

public:
    NotificationController();    
    void setClipboardBusy(bool busy);
    void setStoreLocked(bool locked);
    void loop();
};

#endif
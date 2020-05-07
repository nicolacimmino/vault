
#ifndef __NOTIFICATION_CONTROLLER_H__
#define __NOTIFICATION_CONTROLLER_H__

#include <Arduino.h>
#include "hardware.h"

#ifdef NEOPIXEL_NOTIFICATION
#include <FastLED.h>
#endif

#define NOTFICATION_LOCKED 1
#define NOTFICATION_CLIPBOARD_BUSY 2
#define NOTFICATION_IDLE 4

class NotificationController
{
private:
    int kfp = 0;
    byte status = 0;        
#ifdef NEOPIXEL_NOTIFICATION
    CRGB led[1];
#endif

public:
    NotificationController();    
    void setClipboardBusy(bool busy);
    void setIdle(bool idle);
    void setStoreLocked(bool locked, int kfp);
    void loop();
};

#endif
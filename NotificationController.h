
#ifndef __NOTIFICATION_CONTROLLER_H__
#define __NOTIFICATION_CONTROLLER_H__

#include <Arduino.h>
#include "hardware.h"

class NotificationController
{
private:
    bool ledRed = false;
    bool ledYellow = false;
    bool ledGreen = false;

public:
    NotificationController();
    void setClipboardArmed(bool armed);
    void setStoreLocked(bool locked);
    void loop();
};

#endif
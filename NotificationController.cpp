#include "NotificationController.h"

NotificationController::NotificationController()
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);

    this->loop();
}

void NotificationController::setClipboardArmed(bool armed)
{
    this->ledYellow = armed;

    this->loop();
}

void NotificationController::setStoreLocked(bool locked)
{
    this->ledRed = locked;
    this->ledGreen = !locked;

    this->loop();
}

void NotificationController::setClipboardBusy(bool busy)
{
    this->ledYellowBlink = busy;
    
    this->loop();
}

void NotificationController::loop()
{
    bool ledYellowState = this->ledYellow && (!this->ledYellowBlink || (this->ledYellowBlink && (millis() % 500) < 100));

    digitalWrite(LED_RED, this->ledRed);
    digitalWrite(LED_YELLOW, ledYellowState);
    digitalWrite(LED_GREEN, this->ledGreen);
}
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

void NotificationController::loop()
{
    digitalWrite(LED_RED, this->ledRed);
    digitalWrite(LED_YELLOW, this->ledYellow);
    digitalWrite(LED_GREEN, this->ledGreen);
}
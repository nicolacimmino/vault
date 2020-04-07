#include "NotificationController.h"

NotificationController::NotificationController()
{
#ifdef LED_NOTIFICATION
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
#endif

#ifdef NEOPIXEL_NOTIFICATION
    FastLED.addLeds<WS2812B, 4, GRB>(this->led, 2);
    FastLED.setBrightness(50);
#endif

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
    bool ledYellowState = this->ledYellowBlink && ((millis() % 1000) < 500);

#ifdef LED_NOTIFICATION
    digitalWrite(LED_RED, this->ledRed);
    digitalWrite(LED_YELLOW, ledYellowState);
    digitalWrite(LED_GREEN, this->ledGreen);
#endif

#ifdef NEOPIXEL_NOTIFICATION

    this->led[0] = CRGB::Black;

    if (this->ledRed)
    {
        this->led[0] = CRGB::Red;
    }

    if (this->ledGreen)
    {
        this->led[0] = CRGB::Green;
    }

    if (ledYellowState)
    {
        this->led[0] = CRGB::Yellow;
    }

    FastLED.show();
#endif
}
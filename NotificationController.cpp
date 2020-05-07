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
    FastLED.setBrightness(255);
#endif

    this->loop();
}

void NotificationController::setStoreLocked(bool locked, int kfp)
{
    if (locked)
    {
        this->status |= NOTFICATION_LOCKED;
        this->kfp = 0;
    }
    else
    {
        this->status &= ~NOTFICATION_LOCKED;
        this->kfp = kfp;
    }

    this->loop();
}

void NotificationController::setClipboardBusy(bool busy)
{
    if (busy)
    {
        this->status |= NOTFICATION_CLIPBOARD_BUSY;
    }
    else
    {
        this->status &= ~NOTFICATION_CLIPBOARD_BUSY;
    }

    this->loop();
}

void NotificationController::setIdle(bool idle)
{
    if (idle)
    {
        this->status |= NOTFICATION_IDLE;
    }
    else
    {
        this->status &= ~NOTFICATION_IDLE;
    }

    this->loop();
}

void NotificationController::loop()
{

#ifdef LED_NOTIFICATION
    digitalWrite(LED_RED, this->status & NOTFICATION_LOCKED);
    digitalWrite(LED_YELLOW, this->status & NOTFICATION_CLIPBOARD_BUSY);
    digitalWrite(LED_GREEN, !(this->status & NOTFICATION_LOCKED));
#endif

#ifdef NEOPIXEL_NOTIFICATION
    this->led[0] = (this->status & NOTFICATION_LOCKED) ? CRGB::DarkBlue : (CRGB)CHSV(this->kfp % 255, 255, 255);

    if (this->status & NOTFICATION_CLIPBOARD_BUSY)
    {
        this->led[0] = CRGB::CRGB(255, 155, 32);
    }

    if (this->status & NOTFICATION_IDLE)
    {
        this->led[0].fadeLightBy((millis()/12) % 255);
    }

    FastLED.show();
#endif
}
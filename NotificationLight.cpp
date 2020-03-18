
#include "NotificationLight.h"

void NotificationLight::setBreathe(bool breathe)
{
    this->breathe = breathe;
}

void NotificationLight::begin()
{
    FastLED.addLeds<WS2812B, 4, GRB>(this->leds, 1);
}

void NotificationLight::setColor(CRGB color)
{
    this->color = color;
    this->loop();
}

void NotificationLight::setBrightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
    this->loop();
}

void NotificationLight::loop()
{
    this->leds[0] = this->color;

    if (this->breathe)
    {
        // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
        // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
        float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
        this->leds[0].fadeLightBy(val);
    }
    else
    {
        this->leds[0].fadeLightBy(0);
    }

    FastLED.show();
}

void NotificationLight::shutdown()
{
    this->setColor(CRGB::Black);
    this->loop();
}
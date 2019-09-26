
#include "FloodLight.h"

void FloodLight::begin()
{
    FastLED.addLeds<WS2812B, 4, GRB>(this->leds, 2);
}

void FloodLight::setColor(CRGB color0, CRGB color1 = NULL)
{
    this->leds[0] = color0;
    this->leds[1] = color1 != (CRGB)NULL ? color1 : color0;

    this->leds[0].fadeLightBy(this->fade0);
    this->leds[1].fadeLightBy(this->fade1);
}

void FloodLight::setBrightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
}

void FloodLight::setFade(uint8_t fade)
{
    this->fade0 = fade;
    this->fade1 = fade;
}

void FloodLight::setFade(uint8_t fade0, uint8_t fade1)
{
    this->fade0 = fade0;
    this->fade1 = fade1;
}

void FloodLight::loop()
{
    FastLED.show();
}
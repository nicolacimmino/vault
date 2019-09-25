
#include "FloodLight.h"

void FloodLight::begin()
{
    FastLED.addLeds<WS2812B, 4, GRB>(this->leds, 2);
}

void FloodLight::setColor(CRGB color0, CRGB color1 = NULL)
{
    this->leds[0] = color0;
    this->leds[1] = color1 != (CRGB)NULL ? color1 : color0;

    this->leds[0].fadeToBlackBy(255 - this->fade);
    this->leds[1].fadeToBlackBy(255 - this->fade);
}

void FloodLight::setBrightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
}

void FloodLight::setFade(uint8_t fade)
{
    this->fade = fade;
}

void FloodLight::loop()
{
    FastLED.show();
}
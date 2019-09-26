
#include "FloodLight.h"

void FloodLight::begin()
{
    FastLED.addLeds<WS2812B, 4, GRB>(this->leds, 2);
}

void FloodLight::setColor(CRGB color)
{
    this->colors[0] = color;
    this->colors[1] = color;
}

void FloodLight::setColor(CRGB color0, CRGB color1)
{
    this->colors[0] = color0;
    this->colors[1] = color1;
}

void FloodLight::setBrightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
}

void FloodLight::setFade(uint8_t fade)
{
    this->fade[0] = fade;
    this->fade[1] = fade;
}

void FloodLight::setFade(uint8_t fade0, uint8_t fade1)
{
    this->fade[0] = fade0;
    this->fade[1] = fade1;
}

void FloodLight::loop()
{
    this->leds[0] = this->colors[0];
    this->leds[0].fadeLightBy(this->fade[0]);

    this->leds[1] = this->colors[1];
    this->leds[1].fadeLightBy(this->fade[1]);

    FastLED.show();
}
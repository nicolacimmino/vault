#ifndef __FLOODLIGHT_H__
#define __FLOODLIGHT_H__

#include <Arduino.h>
#include <FastLED.h>

class FloodLight
{
public:
    void begin();
    void loop();
    void setColor(CRGB color0, CRGB color1 = NULL);
    void setBrightness(uint8_t brightness);
    void setFade(uint8_t fade);
    void setFade(uint8_t fade0, uint8_t fade1);
private:
    CRGB leds[2];
    uint8_t fade0 = 0;
    uint8_t fade1 = 0;
};

#endif
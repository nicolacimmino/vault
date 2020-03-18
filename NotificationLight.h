#ifndef __NOTIFICATION_LIGHT_H__
#define __NOTIFICATION_LIGHT_H__

#include <Arduino.h>
#include <FastLED.h>

class NotificationLight
{
private:
    CRGB leds[1];
    CRGB color;
    bool breathe = false;

public:
    void begin();
    void loop();
    void setColor(CRGB color);
    void setBrightness(uint8_t brightness);
    void shutdown();
    void setBreathe(bool breathe);
};

#endif
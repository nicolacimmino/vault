#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <Arduino.h>

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define AXIS_LEVEL 0
#define AXIS_POSITIVE_TILT 1
#define AXIS_NEGATIVE_TILT 2

class Accelerometer
{
public:
    void begin(uint8_t pinXAxes, uint8_t pinYAxes, uint8_t pinZAxes,
               void (*onTiltX)(bool positive) = NULL, void (*onTiltY)(bool positive) = NULL, void (*onTiltZ)(bool positive) = NULL,
               void (*onShake)() = NULL);
    int16_t getX();
    int16_t getY();
    int16_t getZ();
    void loop();
    int16_t getAveragedX();
    int16_t getAveragedY();
    int16_t getAveragedZ();

private:
    uint8_t pinAxes[3];
    uint8_t axesStatus[3];
    int16_t getAxis(uint8_t axis);
    int16_t calibationTable[6] = {
        250, 730, // X min, max
        300, 790, // Y min, max
        190, 680  // Z min, max
    };
    int16_t calibationTableAxisShake[6] = {
        80, 1000, // X min, max
        80, 1000, // Y min, max
        80, 1000  // Z min, max
    };
    int16_t averagedAxis[3] = {0, 0, 0};
    unsigned long movingAverageSampleTime = 0;
    unsigned long lastShakeTime = 0;
    void senseAxisTiltMotion(uint8_t axis, int16_t axisTilt);
    void senseAxisShakeMotion(uint8_t axis);
    void (*onTilt[3])(bool positive);
    void (*onShake)();
};

#endif

#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include "Arduino.h"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

class Accelerometer
{
public:
    void begin(uint8_t pinXAxes, uint8_t pinYAxes, uint8_t pinZAxes);
    int16_t getX();
    int16_t getY();
    int16_t getZ();
private:
    uint8_t pinAxes[3];
    int16_t getAxis(uint8_t axis);
    int16_t calibationTable[6] = {
        250, 730, // X min, max
        300, 790, // Y min, max
        190, 680  // Z min, max
    };
};

#endif


#include "Accelerometer.h"

void Accelerometer::begin(uint8_t pinXAxes, uint8_t pinYAxes, uint8_t pinZAxes)
{
    this->pinAxes[X_AXIS] = pinXAxes;
    this->pinAxes[Y_AXIS] = pinYAxes;
    this->pinAxes[Z_AXIS] = pinZAxes;

    pinMode(pinXAxes, INPUT);
    pinMode(pinYAxes, INPUT);
    pinMode(pinZAxes, INPUT);
}

int16_t Accelerometer::getX()
{
    return this->getAxis(X_AXIS);
}

int16_t Accelerometer::getY()
{
    return this->getAxis(Y_AXIS);
}

int16_t Accelerometer::getZ()
{
    return this->getAxis(Z_AXIS);
}

int16_t Accelerometer::getAxis(uint8_t axis)
{
    int16_t acc = analogRead(this->pinAxes[axis]);

    int16_t calMin = this->calibationTable[axis * 2];
    int16_t calMax = this->calibationTable[(axis * 2) + 1];

    acc = min(acc, calMax);
    acc = max(acc, calMin);

    return ((acc - calMin) / ((calMax - calMin) / 250.0)) - 127;
}


#include "Accelerometer.h"

void Accelerometer::begin(uint8_t pinXAxes, uint8_t pinYAxes, uint8_t pinZAxes, void (*onTiltX)(bool positive) = NULL, void (*onTiltY)(bool positive) = NULL, void (*onTiltZ)(bool positive) = NULL)
{
    this->pinAxes[X_AXIS] = pinXAxes;
    this->pinAxes[Y_AXIS] = pinYAxes;
    this->pinAxes[Z_AXIS] = pinZAxes;

    pinMode(pinXAxes, INPUT);
    pinMode(pinYAxes, INPUT);
    pinMode(pinZAxes, INPUT);

    this->onTilt[X_AXIS] = onTiltX;
    this->onTilt[Y_AXIS] = onTiltY;
    this->onTilt[Z_AXIS] = onTiltZ;
}

void Accelerometer::loop()
{
    if (millis() - this->movingAverageSampleTime > 100)
    {
        this->movingAverageSampleTime = millis();

        for (int ix = 0; ix < 3; ix++)
        {
            int16_t axisTilt = this->getAxis(ix);

            this->averagedAxis[ix] = ((float)this->averagedAxis[ix] * 0.9) + axisTilt * 0.1;

            this->senseAxisTiltMotion(ix, axisTilt);
        }
    }
}

void Accelerometer::senseAxisTiltMotion(uint8_t axis, int16_t axisTilt)
{
    if (this->onTilt[axis] == NULL)
    {
        return;
    }

    if (axisTilt < -96 && this->axesStatus[axis] == AXIS_LEVEL)
    {
        this->axesStatus[axis] = AXIS_NEGATIVE_TILT;
        this->onTilt[axis](false);
    }
    else if (axisTilt > 96 && this->axesStatus[axis] == AXIS_LEVEL)
    {
        this->axesStatus[axis] = AXIS_POSITIVE_TILT;
        this->onTilt[axis](true);
    }
    else if (axisTilt < 32 && axisTilt > -32 && this->axesStatus[axis] != AXIS_LEVEL)
    {
        this->axesStatus[axis] = AXIS_LEVEL;
    }
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

int16_t Accelerometer::getAveragedX()
{
    return this->averagedAxis[X_AXIS];
}

int16_t Accelerometer::getAveragedY()
{
    return this->averagedAxis[Y_AXIS];
}

int16_t Accelerometer::getAveragedZ()
{
    return this->averagedAxis[Z_AXIS];
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

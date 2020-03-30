// Secondary noise source pulls 32 bits of entropy from the internal time jitter.
//
//  Copyright (C) 2020 Nicola Cimmino
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "NoiseSource.h"

NoiseSource::NoiseSource()
{
    // Setup a watchdog interrupt every 64mS.
    cli();
    _WD_CONTROL_REG = (1 << WDCE) | (1 << WDE);
    _WD_CONTROL_REG = (1 << WDIE) | (1 << WDP1);
    sei();
}

NoiseSource *NoiseSource::theInstance = NULL;

NoiseSource *NoiseSource::instance()
{
    if (!theInstance)
    {
        theInstance = new NoiseSource();
    }

    return theInstance;
}

/**
 * If your project is using the ISR you will get compilation errors.
 * Define __ISR_GLOBAL_DEFINED__ and copy the body below into your global ISR function.
 */
#ifndef __ISR_GLOBAL_DEFINED__

/**
 * This is the Arduino Interrupt Service Routine, which will be 
 * called by the WDT interrupt every 64ms.
 */
ISR(WDT_vect)
{
    NoiseSource::instance()->collectNoise();
}

#endif

/**
 * This is called by an ISR driven by the timer and collects at every round
 * one bit of entropy. We sample here the current value of the LSB of Timer1 
 * counter. The jitter of the interrupt process makes this random. 
 */
void NoiseSource::collectNoise()
{
    static byte noiseBuffer = 0;
    static byte collectedBits = 0;

    noiseBuffer = (noiseBuffer << 1) | (TCNT1L & 1);
    collectedBits++;

    if (collectedBits == 8)
    {
        collectedBits = 0;

        if (this->randomNumberPoolIx >= NOISE_SOURCE_POOL_SIZE)
        {
            return;
        }

        /**
          * We pass here the collected 8 bits into a CRC-32, note that this
          * CRC-32 is forever running so it acts as randomness extractor and
          * de-bias of the input source.
        */
        this->crc.update(noiseBuffer & 0xFF);
        this->randomNumberPool[this->randomNumberPoolIx] = this->crc.finalize() & 0xFF;

        this->randomNumberPoolIx++;

        this->randomNumberReady = true;
    }
}

bool NoiseSource::isRandomNumberReady()
{
    return this->randomNumberPoolIx > 0;
}

byte NoiseSource::getRandomNumber()
{
    if (this->randomNumberPoolIx == 0)
    {
        return 0;
    }

    this->randomNumberPoolIx--;

    return this->randomNumberPool[this->randomNumberPoolIx];
}

byte NoiseSource::getRandomPoolFillStatus()
{
    return (100 *this->randomNumberPoolIx) / NOISE_SOURCE_POOL_SIZE;
}
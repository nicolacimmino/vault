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

#ifndef __PGEN_NOISE_SOURCE_H__
#define __PGEN_NOISE_SOURCE_H__

#include <avr/wdt.h>
#include <CRC32.h>

#define NOISE_SOURCE_POOL_SIZE 32

class NoiseSource
{
public:
    static NoiseSource* instance();    
    void collectNoise();
    bool isRandomNumberReady();
    byte getRandomNumber();
    byte getRandomPoolFillStatus();

private:
    NoiseSource();
    static NoiseSource* theInstance;        
    CRC32 crc;    
    bool randomNumberReady = false;
    byte randomNumberPool[NOISE_SOURCE_POOL_SIZE];
    byte randomNumberPoolIx = 0;
};

#endif
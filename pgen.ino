
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

#include <AES.h>
//#include "./printf.h"
#include "NoiseSource.h"
#include "EncryptedStore.h"

AES aes;

byte *key = (unsigned char *)"0123456789010123";

byte plain[] = "A Test string";
int plainLength = sizeof(plain) - 1; // don't count the trailing /0 of the string !
int padedLength = plainLength + N_BLOCK - plainLength % N_BLOCK;

//real iv = iv x2 ex: 01234567 = 0123456701234567
unsigned long long int my_iv = 36753562;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    //printf_begin();
    delay(500);
    //printf("\n===testing mode\n");

    //  otfly_test () ;
    //  otfly_test256 () ;
}

EncryptedStore encryptedStore;

void loop()
{    
 
    byte key[] = {
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
    };
    encryptedStore.init(key);
    encryptedStore.set(0, plainLength, plain);

    char decrypted[100];

    encryptedStore.get(0, decrypted);
    Serial.println(decrypted);

    return;
    // if (NoiseSource::instance()->isRandomNumberReady())
    // {
    //     Serial.println(NoiseSource::instance()->getRandomNumber(), HEX);
    // }
    //prekey_test () ;
    //delay(2000);
}

void prekey(int bits)
{
    aes.iv_inc();
    byte iv[N_BLOCK];
    byte plain_p[padedLength];
    byte cipher[padedLength];
    byte check[padedLength];
    unsigned long ms = micros();
    aes.set_IV(my_iv);
    aes.get_IV(iv);
    aes.do_aes_encrypt(plain, plainLength, cipher, key, bits, iv);
    Serial.print("Encryption took: ");
    Serial.println(micros() - ms);
    ms = micros();
    aes.set_IV(my_iv);
    aes.get_IV(iv);
    aes.do_aes_decrypt(cipher, padedLength, check, key, bits, iv);
    Serial.print("Decryption took: ");
    Serial.println(micros() - ms);
    //printf("\n\nPLAIN :");
    aes.printArray(plain, (bool)true);
    //printf("\nCIPHER:");
    aes.printArray(cipher, (bool)false);
    ///printf("\nCHECK :");
    aes.printArray(check, (bool)true);
    //printf("\nIV    :");
    aes.printArray(iv, 16);
    //printf("\n============================================================\n");
}

void prekey_test()
{
    prekey(128);
}

/*
#include <VT100.h>
#include "messages.h";

void setup()
{
    Serial.begin(9600);
    VT100.begin(Serial);
    VT100.cursorSave();
}

char buffer[255];

void printMessage(uint8_t messageId)
{
    strcpy_P(buffer, (char *)pgm_read_word(&(messages[messageId])));
    Serial.print(buffer);
}

void loop()
{
    while(Serial.available()) {
        Serial.println((byte)Serial.read());
    }
    return;
    if (Serial.available())
    {
        char c = Serial.read();

        if (c == '0')
        {
            VT100.setBackgroundColor(VT_BLACK);
            VT100.clearScreen();

            VT100.setBackgroundColor(VT_BLACK);
            VT100.setTextColor(VT_GREEN);
            VT100.setCursor(1, 1);

            printMessage(0);

            VT100.setCursor(9, 1);

            printMessage(1);
        }

        if (c == '1')
        {
            VT100.setCursor(9, 1);
            printMessage(2);
        }
    }
}*/
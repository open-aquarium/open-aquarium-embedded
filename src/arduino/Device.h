#ifndef OA_DEVICE_H
#define OA_DEVICE_H

class Device
{
    public:
    // double x;
    // double y;

    // virtual double area() = 0;
    int getFreeSRAM();
    // int getFreeFlash();
    // int getFreeEEPROM();
};

#endif

/*
ATmega328P
Flash  32k bytes (of which .5k is used for the bootloader)
SRAM   2k bytes
EEPROM 1k byte

ATmega2560
Flash  256k bytes (of which 8k is used for the bootloader)
SRAM   8k bytes
EEPROM 4k byte
*/
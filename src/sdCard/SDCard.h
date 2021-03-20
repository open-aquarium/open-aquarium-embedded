#ifndef OA_SDCARD_H
#define OA_SDCARD_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

class SDCard {

  private:

    byte pin;

    Sd2Card card;

    SdVolume volume;

    bool ready = false;

    const String DEFAULT_DATA_FILE = "data.txt";

    const String LOG_FILE = "log.txt";

    const String ERROR_FILE = "error.txt";

    const String DEBUG_FILE = "debug.txt";

    void println(String file, String message);

    void dumpFile(String filename);

  public:

    SDCard(byte pin);

    String cardType();

    uint32_t clusterCount();

    uint8_t blocksPerCluster();

    uint32_t totalBlocks();

    String volumeType();

    uint32_t volumeSize();

    void println(String message);

    void printLog(String message);

    void printError(String message);

    void printDebug(String message);

    void dumpFile();

    void dumpLogFile();

    void dumpErrorFile();

    void dumpDebugFile();

};

#endif

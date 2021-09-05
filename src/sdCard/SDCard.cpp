#include "SDCard.h"

SDCard::SDCard(byte pin) {
  this->pin = pin;
  if (!card.init(SPI_HALF_SPEED, pin)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
  } else {
    this->ready = true;
  }
  if (!volume.init(card)) {
    this->ready = false;
  }
  if (!SD.begin(pin)) {
    this->ready = false;
  }
}

String SDCard::cardType() {
  if(!this->ready) {
    return F("SD CARD NOT READY");
  }
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      return F("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      return F("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      return F("SDHC");
      break;
    default:
      return F("Unknown");
  }
}

uint32_t SDCard::clusterCount() {
  return this->volume.clusterCount();
}

uint8_t SDCard::blocksPerCluster() {
  return this->volume.blocksPerCluster();
}

uint32_t SDCard::totalBlocks() {
  return this->volume.blocksPerCluster() * this->volume.clusterCount();
}

String SDCard::volumeType() {
  if(!this->ready) {
    return F("ERROR");
  }
  if (!this->volume.init(card)) {
    return F("UNKNOWN");
  }
  String type = F("FAT");
  return type + this->volume.fatType();
}

uint32_t SDCard::volumeSize() {
  return volume.blocksPerCluster() * volume.clusterCount() / 2 * 1024;
}

uint32_t SDCard::freeSpace() {
  // TODO calculate free space on the SD Card
  // https://forum.arduino.cc/t/sd-card-read-available-space/136548/3
  return 0;
}

void SDCard::println(String message) {
  this->println(this->DEFAULT_DATA_FILE, message);
}

void SDCard::println(String file, String message) {
  File myFile = SD.open(file, FILE_WRITE);
  if (myFile) {
    myFile.println(message);
    myFile.close();
  } else {
    Serial.println(F("error opening log file"));
  }
}

void SDCard::printLog(String message) {
  this->println(LOG_FILE, message);
}

void SDCard::printError(String message) {
  this->println(ERROR_FILE, message);
}

void SDCard::printDebug(String message) {
  this->println(DEBUG_FILE, message);
}

void SDCard::dumpFile(String filename) {
  File myFile = SD.open(filename);
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error opening the file");
  }
}

void SDCard::dumpFile() {
  this->dumpFile(this->DEFAULT_DATA_FILE);
}

void SDCard::dumpLogFile() {
  this->dumpFile(this->LOG_FILE);
}

void SDCard::dumpErrorFile() {
  this->dumpFile(this->ERROR_FILE);
}

void SDCard::dumpDebugFile() {
  this->dumpFile(this->DEBUG_FILE);
}

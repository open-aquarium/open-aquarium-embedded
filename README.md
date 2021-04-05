# Open Aquarium - Embedded

## Dependencies

| Equipment    | Lib                                                                           | Version | PIN | Notes |
|--------------|-------------------------------------------------------------------------------|---------|-----|-------|
| DHT22        | [adafruit/DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library) | 1.4.2   |     |       |
|              | [ArduinoJson](https://arduinojson.org)                                        | 6.17.3  |     |       |
|              | [SD Lib](https://www.arduino.cc/en/Reference/SD)                              | 1.2.4   |     |       |
| DS3231       | [RTClib](https://github.com/adafruit/RTClib)                                  | 1.12.5  |     |       |
| OLED Display | [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)      | 1.10.6  |     |       |
| OLED Display | [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)              | 2.4.3   |     |       |
| BMP280       | [Adafruit BMP280](https://github.com/adafruit/Adafruit_BMP280_Library)        | 2.1.1   |     |       |
| esp8266      | [WifiEsp](https://github.com/bportaluri/WiFiEsp)                              | 2.2.2   |     |       |

## Data Types

| Block             | Field                  | Type     | Min | Max | Unit    | Notes                                      |
|-------------------|------------------------|----------|-----|-----|---------|--------------------------------------------|
| header            | type                   | String   |     |     |         | Event type                                 |
| header            | eventId                | String   |     |     |         | Event unique universal ID (UUID)           |
| header            | triggerTime            | String   |     |     |         | Date/Time when the event occured           |
| device            | serialNumber           | String   |     |     |         | Hardware serial number                     |
| device            | softwareVersion        | String   |     |     |         | Software version                           |
| device            | hardwareVersion        | String   |     |     |         | Hardware version                           |
| rollCall.sensor   | id                     | int      |     |     |         | Sequential number                          |
| rollCall.sensor   | sensor                 | String   |     |     |         | Sensor name                                |
| rollCall.sensor   | address                | String   |     |     |         | Sensor address, pin                        |
| rollCall.sensor   | category               | String   |     |     |         | Sensor category                            |
| deviceSample      | freeMemory             | uint8_t  |     |     | Byte    | Free SRAM memory                           |
| deviceSample      | sdCardType             | String   |     |     |         | SD, SD2                                    |
| deviceSample      | sdCardVolumeType       | String   |     |     |         | FAT, FAT32                                 |
| deviceSample      | sdCardVolumeSize       | uint32_t |     |     | Byte    | MicroSD Volume size                        |
| deviceSample      | sdCardClusterCount     | uint32_t |     |     | Unit    | Number of clusters                         |
| deviceSample      | sdCardBlocksPerCluster | uint8_t  |     |     | Unit    | Number of blocks per cluster               |
| deviceSample      | sdCardTotalBlocks      | uint32_t |     |     | Unit    | Total number of blocks                     |
| deviceSample      | temperature            | float    |     |     | Celcius | Device internal temperature                |
| environmentSample | roomTemperature        | float    |     |     | Celcius | Room temperature (DHT22)                   |
| environmentSample | roomTemperature2       | float    |     |     | Celcius | Room temperature (BMP280)                  |
| environmentSample | relativeHumidity       | float    |     |     | %       | Relative Humidity                          |
| environmentSample | heatIndex              | float    |     |     | Celcius | Human-perceived equivalent temperature     |
| environmentSample | atmosphericPressure    | float    |     |     | Pascal  | Atmosferic pressure                        |
| environmentSample | altitude               | float    |     |     | Meters  | Relative altitude related to the sea level |
| environmentSample | lightIntensity         | float    |     |     | ?       | Light intensity                            |
| environmentSample | noiseLevel             | float    |     |     | ?       | Noise level                                |

## Examples

* https://www.youtube.com/watch?v=kPLafcrng-c

## Good to have

* [DETECT THE BOARD DATA](https://tonygaitatzis-blog.tumblr.com/post/134967126657/determine-arduino-board-programmatically)
* [EEPROM](https://www.arduino.cc/en/Tutorial/LibraryExamples/EEPROMWrite)

## Fritzing

- [Adafruit parts (DHT22)](https://github.com/adafruit/Fritzing-Library/blob/master/AdaFruit.fzbz)

## References

### I2C Protocol

- [Beginning with Arduino and I2c Protocol (pt_BR)](https://cadernodelaboratorio.com.br/iniciando-com-o-arduino-o-protocolo-i2c/)

### Led

- https://www.electronica-pt.com/led

### Resistor

- https://www.mouser.com/technical-resources/conversion-calculators/resistor-color-code-calculator

### DHT

- https://learn.adafruit.com/dht

### OLED Display

- https://www.arduinoecia.com.br/tutorial-display-oled-arduino-ssd1306/
- https://lastminuteengineers.com/oled-display-arduino-tutorial/

### Photocells

- https://learn.adafruit.com/photocells/overview


### SD Card

- https://randomnerdtutorials.com/guide-to-sd-card-module-with-arduino/
- https://www.usinainfo.com.br/blog/projeto-arduino-sd-card-leitura-e-escrita-de-dados-no-cartao-micro-sd/
- https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/
- https://blogmasterwalkershop.com.br/arduino/como-usar-com-arduino-modulo-leitor-de-micro-sd-card

### Real Time Clock (RTC)

- https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-real-time-clock-module-ds3231-bc90fe
- https://learn.adafruit.com/adafruit-ds3231-precision-rtc-breakout/arduino-usage
- https://learn.adafruit.com/adafruit-pcf8523-real-time-clock/rtc-with-arduino

### Barometric Pressure (BMP280)

* [Barometric Pressure, Temperature and Altitude](https://www.arduinoecia.com.br/bmp280-pressao-temperatura-altitude/)

### Sound Sensor

- [Sound Module](https://pt.aliexpress.com/item/32530266337.html?spm=a2g0o.detail.0.0.6c603fd2kr8CLq&gps-id=pcDetailBottomMoreThisSeller&scm=1007.13339.169870.0&scm_id=1007.13339.169870.0&scm-url=1007.13339.169870.0&pvid=20c5b25b-54bc-4fd5-96ca-1a44b0beb038&_t=gps-id:pcDetailBottomMoreThisSeller,scm-url:1007.13339.169870.0,pvid:20c5b25b-54bc-4fd5-96ca-1a44b0beb038,tpp_buckets:668%230%23131923%2367_668%230%23131923%2367_668%23888%233325%2313_668%23888%233325%2313_668%232846%238111%231996_668%232717%237566%23838_668%231000022185%231000066058%230_668%233468%2315615%23686_668%232846%238111%231996_668%232717%237566%23838_668%233164%239976%23243_668%233468%2315615%23686)

### Other

* https://en.cppreference.com/w/cpp/types/integer

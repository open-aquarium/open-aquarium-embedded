# Open Aquarium - Embedded

## Examples

* https://www.youtube.com/watch?v=kPLafcrng-c

## Good to have

* [DETECT THE BOARD DATA](https://tonygaitatzis-blog.tumblr.com/post/134967126657/determine-arduino-board-programmatically)


## Fritzing

- [Adafruit parts (DHT22)](https://github.com/adafruit/Fritzing-Library/blob/master/AdaFruit.fzbz)

## References

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

# Real Time Clock (RTC)

- https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-real-time-clock-module-ds3231-bc90fe
- https://learn.adafruit.com/adafruit-ds3231-precision-rtc-breakout/arduino-usage
- https://learn.adafruit.com/adafruit-pcf8523-real-time-clock/rtc-with-arduino

## Release Workflow

Recommended workflow

- Make changes
- Commit those changes
- Make sure CI turns green
- Bump version in package.json
- Generate Changelog
- Commit package.json and CHANGELOG.md files
- Tag
- Push

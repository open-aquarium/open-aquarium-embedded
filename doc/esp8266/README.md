# ESP8266

## Check chip

```
$ python3 -m esptool -p /dev/ttyUSB0 flash_id
```

## Flashing AT Firmware

```
$ python3 -m esptool --port /dev/ttyUSB0 --baud 115200 write_flash --flash_size 4MB --flash_mode dio --flash_freq 40m 0x00000 boot_v1.2.bin 0x01000 at/512+512/user1.1024.new.2.bin 0x7e000 blank.bin 0x3fe000 blank.bin 0x3fc000 esp_init_data_default.bin
```

## AT Commands Samples

```
AT

AT+RST

AT+GMR

AT+CWMODE=1

AT+CWLAP

AT+CWJAP="ssid","passwd"

AT+CIFSR

AT+PING="google.com"

AT+CWQAP
```

## References

- [esp8266 nonos sdk](https://github.com/espressif/ESP8266_NONOS_SDK/releases)
- [ESP-12F-Flashing-AT-Firmware](https://www.instructables.com/ESP-12F-Flashing-AT-Firmware/)
- [How to Flash the Latest Firmware into the esp8266 board in linux using esptools](https://karibe.co.ke/2018/10/how-to-flash-the-latest-firmware-into-the-esp8266-board-in-linux-using-esptools/)
- [Arduino Mega WiFi (AtMega2560 + ESP8266)](https://robotdyn.com/mega-wifi-r3-atmega2560-esp8266-flash-32mb-usb-ttl-ch340g-micro-usb.html)
- [No detalhe! Arduino Mega ESP8266 - Review - Internet e Coisas #118](https://www.youtube.com/watch?v=FV8TN7BZxBw)
- [Guia do usuário do esp8266](https://www.filipeflop.com/blog/guia-do-usuario-do-esp8266/)
- [esp8266 Arduino tutorial](https://www.filipeflop.com/blog/esp8266-arduino-tutorial/)
- [Upgrade de firmware do módulo esp8266](https://www.filipeflop.com/blog/upgrade-de-firmware-do-modulo-esp8266/)
- [Atualização de flash na esp-01](https://www.embarcados.com.br/atualizacao-de-flash-na-esp-01/)
- [Update flash esp8266 firmware](https://www.electronicshub.org/update-flash-esp8266-firmware/)
- [Como conectar o esp8266 à internet](https://www.filipeflop.com/blog/como-conectar-o-esp8266-a-internet/)

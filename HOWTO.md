# FLASH PREPROCESSOR FIRMWARE
- Locate Firmware in [firmware/](firmware/)
- (re)connect Uno via USB-B
- put AtMega16u2 in [DFU mode](https://support.arduino.cc/hc/en-us/articles/4408887452434-Flash-USB-to-serial-firmware-in-DFU-mode).
- use the following commands to
  1. read FLASH (for connection verification.)
  2. erase EEPROM
  3. upload flash
  4. reset board
```
dfu-programmer atmega16u2 read
dfu-programmer atmega16u2 erase
dfu-programmer atmega16u2 flash --debug 1 firmware/INSERT_FIRMWARE_NAME_HERE.hex  
dfu-programmer atmega16u2 reset  
```

# FLASH MICROCONTROLLER
- connect Programmer via ISP interface (GND is facing down)
- connect Uno via USB-B **to the same PC** (common ground) to power the board
## FLASH USING PLATFORMIO
- use the following `platformio.ini` profile to upload:
```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
upload_protocol = custom
upload_port = usb
upload_flags =
    -C
    ; use "tool-avrdude-megaavr" for the atmelmegaavr platform
    ${platformio.packages_dir}/tool-avrdude/avrdude.conf
    -p
    $BOARD_MCU
    -P
    $UPLOAD_PORT
    -c
    stk500v2
``` 
## FLASH USING ARDUINO IDE
- Select `Atmel STK500 development board` from `Tools->Programmer`
- Select `Arduino Uno` from `Tools->Board`
- **When uploading, make sure to press <kbd>SHIFT</kbd>!**

# COMPILING FIRMWARE (example: arduino-keyboard)
- navigate to [firmware/arduino-keyboard](firmware/arduino-keyboard)
- execute
```bash
make clean; make
```
- to upload, see [this](#FLASH-PREPROCESSOR-FIRMWARE) section.
# DMX_DollHouse
DMX Controller for Haunted Doll House

# Development Environment Setup
1. Install Github Desktop from https://desktop.github.com/download/
2. Open Github Desktop and sign in. Create account if needed.
3. In Github Desktop clone this repo
   1. "File --> Clone repository..."
   2. Select "URL" tab
   3. Enter "rockethacker/DMX_DollHouse" into URL field
   4. Note that this will clone to your documents folder
5. Install Arduino IDE from https://www.arduino.cc/en/software/#ide
6. Open Arduino IDE
7. Install board definition
   1. Open "Tools --> Board --> Boards Manager..."
   2. search for "esp"
   3. Install "esp32 by Espressif Systems" version 3.1.0
   4. If it times out, try this https://forum.arduino.cc/t/downloading-esp32-3-3-5-fails/1420739/7
8. Note that the above installations should take care of the CH340 driver installation. If not, follow https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers
9. Install Libraries
   1. Open "Tools --> Manage Libraries..."
   1. Install "esp_dmx by Mieth Weisbrod" version 4.1.0
      1. dmx read example won't compile, https://github.com/someweisguy/esp_dmx/issues/181#issuecomment-2619261884
      1. update uart.c to fork version
   1. Install "Freenove WS2812 Lib for ESP32" version 2.0.1
   2. Install "SparkFun Qwiic Relay Arduino Library" version 1.3.1

10. Plug in Sparkfun Thing Plus via USB C cable
11. Select board by selecting "Tools --> Board --> esp32 --> SparkFun ESP32 Thing Plus C" 
12. Select port by selecting "Tools -->  Port --> COMxx" Where "xx" is the port your board is connected to (e.g. COM3)
13. Open this project by selecting "File --> Open..." and navigating to "Documents\GitHub\DMX_DollHouse\DMX_DollHouse.ino"

# Hardware
Parts are mostly from SparkFun. If you're in the DFW area and buy from Mouser you can get very fast, cheap delivery (sometimes overnight) via USPS
* https://www.sparkfun.com/sparkfun-esp32-thing-plus-dmx-to-led-shield.html
* https://www.sparkfun.com/sparkfun-thing-plus-esp32-wroom-usb-c.html
* https://www.sparkfun.com/feather-stackable-header-set.html
  * headers for thing plus
* https://www.adafruit.com/product/6313 - FeatherWing 128x64 OLED with pins
* https://www.sparkfun.com/sparkfun-qwiic-quad-relay.html
  * Discontinued product but available on Mouser
* This code supports WS2812 LEDs
* Power connection is a 5.5mm barrel jack with positive center pin.

# Pin & Channel Designations
* Pinouts for SparkFun Thing (SFT) Plus C. J1 is the 16 pin header. J7 is the 12 pin header.
* For some reason, the DMX board documentation had the D1/D1 pins swapped

| SFT Pin | SFT GPIO | SFT Name | Connected To |
| ------- | -------- | -------- | ------------ |
| J1-13   | 19       | POCI     | DMX D0       |
| J1-12   | 23       | PICO     | DMX D1       |
| J7-6    | 27       | 11       | DMX D2       |

# DMX Universe Channels
* All Channels are Offset by the the starting address (default is 0 offset)
* Lighting values are 0-255
* Relay commands are boolean >127 is TRUE

| Chan | Function    |
| ---- | ----------- |
| 1    | LED0 Red    |
| 2    | LED0 Green  |
| 3    | LED0 Blue   |
| 4    | LED0 Dimmer |
| 5    | LED1 Red    |
| 6    | LED1 Green  |
| 7    | LED1 Blue   |
| 8    | LED1 Dimmer |
| 9    | LED2 Red    |
| 10   | LED2 Green  |
| 11   | LED2 Blue   |
| 12   | LED2 Dimmer |
| 13   | Relay 1     |
| 14   | Relay 2     |
| 15   | Relay 3     |
| 16   | Relay 4     |

# Troubleshooting Notes
* "SparkFun DMX Shield Library" makes some bad assumptions on how to detect the beginning of the DMX frames so isn't reliabloe for DMX reading

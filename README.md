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
7. Install Libraries
   1. Open "Tools --> Manage Libraries..."
   2. Install "SparkFun DMX Shield Library"
8. Install board definition
   1. Open "Tools --> Board --> Boards Manager..."
   2. search for "esp"
   3. Install "esp32 by Espressif Systems"
   4. If it times out, try this https://forum.arduino.cc/t/downloading-esp32-3-3-5-fails/1420739/7
9. Note that the above installations should take care of the CH340 driver installation. If not, follow https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers
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

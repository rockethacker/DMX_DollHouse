# DMX_DollHouse
DMX Controller for Haunted Doll House

# Development Environment Setup
1. Install Github Desktop from https://desktop.github.com/download/
2. Open Github Desktop and sign in. Create account if needed.
3. In Github Desktop close this repo
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
9. Select board by selecting "Tools --> Board --> esp32 --> SparkFun ESP32 Thing Plus C" 
10. Note that the above installations should take care of the CH340 driver installation. If not, follow https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers
11. Plug in Sparkfun Thing Plus via USB C cable
12. 

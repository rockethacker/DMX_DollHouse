# DMX_DollHouse
DMX Controller for Haunted Doll House

# Development Environment Setup
1. Install Arduino IDE from https://www.arduino.cc/en/software/#ide
2. Open Arduino IDE
3. Install Libraries
   1. Open "Tools --> Manage Libraries..."
   2. Install "SparkFun DMX Shield Library"
5. Install board definition
   1. Open "Tools --> Board --> Boards Manager..."
   2. search for "esp"
   3. Install "esp32 by Espressif Systems"
6. Select board by selecting "Tools --> Board --> esp32 --> SparkFun ESP32 Thing Plus C" 
7. Note that the above installations should take care of the CH340 driver installation. If not, follow https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers

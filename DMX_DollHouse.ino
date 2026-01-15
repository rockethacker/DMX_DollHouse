/* https://github.com/rockethacker/DMX_DollHouse

== Description ==
Inputs DMX commands and applies to LED, servo and Relay outputs

== Resources ==
https://learn.sparkfun.com/tutorials/installing-an-arduino-library
https://learn.adafruit.com/assets/139874
https://www.youtube.com/shorts/y7VVxOw890M - How to properly press boot button to upload code


== Hardware ==
https://www.sparkfun.com/sparkfun-esp32-thing-plus-dmx-to-led-shield.html
https://www.sparkfun.com/sparkfun-thing-plus-esp32-wroom-usb-c.html
https://www.sparkfun.com/feather-stackable-header-set.html - headers for thing plus
https://www.adafruit.com/product/6313 - FeatherWing 128x64 OLED with pins
https://www.sparkfun.com/sparkfun-qwiic-quad-relay.html - Discontinued product but available on Mouser



== Libraries ==
- SparkFun DMX Shield Library https://github.com/sparkfun/SparkFunDMX

*/

// Inlcude DMX library
//#include "libraries\SparkFun_DMX_Shield_Library\SparkFunDMX.h"
#include "SparkFunDMX.h"

// Create DMX object
SparkFunDMX dmx;

// Create serial port to be used for DMX interface. Exact implementation depends
// on platform, this example is for the ESP32
HardwareSerial dmxSerial(2);  //originally was set to 2, but that doesn't seem right

// Enable pin for DMX shield (Free pin on Thing Plus or Feather pinout)
uint8_t enPin = 4;

// Number of DMX channels, can be up to 512
uint16_t numChannels = 192;

// Buffer to store DMX data
#define buffer_size 16
uint8_t buffer[buffer_size];


void setup()
{
    Serial.begin(115200);
    Serial.println("SparkFun DMX Example 2 - Input");

    // Begin DMX serial port
    // RX and TX pins explicitly defined for ESP32 Thing Plus
    dmxSerial.begin(DMX_BAUD, DMX_FORMAT,16,17);
    //dmxSerial.begin(DMX_BAUD, DMX_FORMAT);

    // Begin DMX driver
    dmx.begin(dmxSerial, enPin, numChannels);

    // Set communicaiton direction, which can be changed on the fly as needed
    dmx.setComDir(DMX_READ_DIR);

    Serial.println("DMX initialized!");
}

void loop()
{
    // Wait until data has been received
    while(dmx.dataAvailable() == false)
    {
        // Must called update() to actually check for received data
        dmx.update();
    }

    // Data has been received, read out channels
    dmx.readBytes(buffer, buffer_size);
    for (uint8_t i = 0; i < buffer_size; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println(" ");
    
/*
    // Data has been received, read out channel 1
    uint8_t data = dmx.readByte(1);
    
    Serial.print("DMX: read value from channel 1: ");
    Serial.println(data);
*/

}
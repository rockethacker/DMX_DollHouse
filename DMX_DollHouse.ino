/* https://github.com/rockethacker/DMX_DollHouse

== Description ==
- Inputs DMX commands and applies to LED, servo and Relay outputs
- Pinouts below are for SparkFun Thing Plus C. Variants of SparkFun Things will have different pin designations, see their graphical datasheet.
- See README for hardware requirements

== Resources ==
https://learn.sparkfun.com/tutorials/installing-an-arduino-library
https://learn.adafruit.com/assets/139874
https://www.youtube.com/shorts/y7VVxOw890M - How to properly press boot button to upload code

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
HardwareSerial dmxSerial(1);  //originally was set to 2, but this board uses port 1, pins 16/17

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
    //dmxSerial.begin(DMX_BAUD, DMX_FORMAT,16,17);
    dmxSerial.begin(DMX_BAUD, DMX_FORMAT);

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
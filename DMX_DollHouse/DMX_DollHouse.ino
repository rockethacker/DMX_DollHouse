/*

  DMX Read

  This sketch allows you to read DMX from a DMX controller using a standard DMX
  shield, such SparkFun ESP32 Thing Plus DMX to LED Shield. This sketch was
  made for the Arduino framework!

  Created 9 September 2021
  By Mitch Weisbrod

  https://github.com/someweisguy/esp_dmx

*/
#include <Arduino.h>
#include <esp_dmx.h>

// Define the hardware pins that interface with the DMX bus
// Pins will differ between ESP32 models
int transmitPin = 17;
int receivePin = 16;
int enablePin = 4;

// Define port connected to DMX. ESP32 has either 2 or 3 ports.
// Port 0 is typically used to transmit serial data back to your Serial Monitor
dmx_port_t dmxPort = 1;

/* Now we want somewhere to store our DMX data. Since a single packet of DMX
  data can be up to 513 bytes long, we want our array to be at least that long.
  This library knows that the max DMX packet size is 513, so we can fill in the
  array size with `DMX_PACKET_SIZE`. */
byte data[DMX_PACKET_SIZE];

bool dmxIsConnected = false; //Connection status
unsigned long lastUpdate = millis(); //timer

//State machine states
enum states {
  DMX_SETUP, DMX_RUN, DMX
};


// a place to store packet information (not data)
dmx_packet_t packet;

void setup() {
  Serial.begin(115200); //Start serial monitor
  Serial.println("Initializing...");

  /* Install the DMX driver. Tell it which DMX port to use,
    what device configuration to use, and what DMX personalities it should have.
    If you aren't sure which configuration to use, you can use the macros
    `DMX_CONFIG_DEFAULT` to set the configuration to its default settings.
    This device is being setup as a DMX responder so it is likely that it should
    respond to DMX commands. It will need at least one DMX personality. Since
    this is an example, we will use a default personality which only uses 1 DMX
    slot in its footprint. */
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_personality_t personalities[] = {
    {1, "Default Personality"}
  };
  int personality_count = 1;
  Serial.println("Installing dmx driver...");
  dmx_driver_install(dmxPort, &config, personalities, personality_count);
  Serial.println("dmx driver installed...");
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);
}

void loop() {

  /* And now we wait! The DMX standard defines the amount of time until DMX
    officially times out. That amount of time is converted into ESP32 clock
    ticks using the constant `DMX_TIMEOUT_TICK`. If it takes longer than that
    amount of time to receive data, this if statement will evaluate to false. */
  if (dmx_receive(dmxPort, &packet, DMX_TIMEOUT_TICK)) {
    
    //we've received DMX data!
    unsigned long now = millis(); //Record when we received new data

    // If no DMX errors, do things
    if (!packet.err) {

      //On first packet, log successful connection
      if (!dmxIsConnected) {

        Serial.println("DMX is connected!");
        Serial.println(packet.size-1, DEC); /list DMX packet size
        dmxIsConnected = true;
      }
      
      // Print results to serial at 1Hz
      if (now - lastUpdate > 1000) {
        
        dmx_read(dmxPort, data, packet.size); //copy data into buffer
        
        /* Print the received start code - it's usually 0. */
        //Serial.printf("Start code is 0x%02X and slot 1 is 0x%02X\n", data[0], data[1]);
        
        // Data has been received, print out 8 channel values
        // Data in slot [0] is start code
        for (uint8_t i = 1; i < 9; i++) {
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        Serial.println(" ");
        
        lastUpdate = now;
      }
    } else {
      /* Oops! A DMX error occurred! Don't worry, this can happen when you first
        connect or disconnect your DMX devices. If you are consistently getting
        DMX errors, then something may have gone wrong with your code or
        something is seriously wrong with your DMX transmitter. */
      Serial.println("A DMX error occurred.");
    }
  } else if (dmxIsConnected) {
    /* If DMX times out after having been connected, it likely means that the
      DMX cable was unplugged. When that happens in this example sketch, we'll
      uninstall the DMX driver. */
    Serial.println("DMX was disconnected.");
    //dmx_driver_delete(dmxPort);

  }
}

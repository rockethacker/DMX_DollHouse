/*

  DMX Read

  This sketch allows you to read DMX from a DMX controller using a standard DMX
  shield, such SparkFun ESP32 Thing Plus DMX to LED Shield. This sketch was
  made for the Arduino framework!

  Created 9 September 2021
  By Mitch Weisbrod

  https://github.com/someweisguy/esp_dmx

*/
// DMX board libs
#include <Arduino.h>
#include <esp_dmx.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"

//Relay Board Libs
#include <Wire.h>
#include "SparkFun_Qwiic_Relay.h"

//Display Libs
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// ==== DMX Init ==== //
// Define DMX bus
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

// ==== LED Init ==== //
#define LEDS_COUNT  100 //Same LED count for all strips
// For some reason the documentation has pins 19 and 23 mixed up
#define LED0_PIN	19 //DMX board terminal D0. J1-pin 13 on Sparkfun Thing 
#define LED1_PIN	23 //DMX board terminal D1. J1-pin 12 on Sparkfun Thing
#define LED2_PIN	27 //DMX board terminal D2. J7-pin 6 on Sparkfun Thing

Freenove_ESP32_WS2812 LED0 = Freenove_ESP32_WS2812(LEDS_COUNT, LED0_PIN, 0, TYPE_GRB);
Freenove_ESP32_WS2812 LED1 = Freenove_ESP32_WS2812(LEDS_COUNT, LED1_PIN, 1, TYPE_GRB);
Freenove_ESP32_WS2812 LED2 = Freenove_ESP32_WS2812(LEDS_COUNT, LED2_PIN, 2, TYPE_GRB);

// ==== Relay Init ==== //
#define RELAY_ADDR 0x6D // Alternate address 0x6C

Qwiic_Relay quadRelay(RELAY_ADDR); 

// ==== Display Init ==== //
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
uint16_t page_chan=0;
uint16_t page=0;
uint8_t data_val=0;

// ==== Program Init ==== //
//State machine states
enum states {
  DMX_SETUP, DMX_RUN, DMX
};

//Starting address in this DMX universe
//This device takes 16 channels
//baby board test is dmx start 1, ion perform is dmx 496
uint16_t dmxStartAddr = 496; 
int rate = 20; //rate in Hz

// a place to store packet information (not data)
dmx_packet_t packet;

void setup() {
  Serial.begin(115200); //Start serial monitor
  Serial.println("Initializing...");

// ==== DMX Init ==== //
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

// ==== LED Init ==== //
  LED0.begin();
  LED0.setBrightness(0);
  LED1.begin();
  LED1.setBrightness(0);
  LED2.begin();
  LED2.setBrightness(0);

// ==== Relay Init ==== //
  Wire.begin(); 
  // Make sure the hardware is set up correctly.
  if(!quadRelay.begin())
    Serial.println("Check connections to Qwiic Relay.");
  else
    Serial.println("Ready to flip some switches.");
  
  // Make sure all relays are off
  quadRelay.turnAllRelaysOff(); 

// ==== Display Init ==== //
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default
  Serial.println("OLED begun");
  display.clearDisplay();
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
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
        Serial.println(packet.size-1, DEC); //list DMX packet size
        dmxIsConnected = true;
      }
      
      // Print results to serial at rate
      if (now - lastUpdate > 1000/rate) {
        
        dmx_read(dmxPort, data, packet.size); //copy data into buffer
        
        /* Print the received start code - it's usually 0. */
        //Serial.printf("Start code is 0x%02X and slot 1 is 0x%02X\n", data[0], data[1]);
        
        // Data has been received, print out channel values to serial
        Serial.print(dmxStartAddr, DEC);
        Serial.print(" : ");
        for (uint16_t i = 0; i < 16; i++) {
          data_val = data[dmxStartAddr+i];
          if (data_val > 0xF){
            Serial.print(data_val, HEX);
          } else {
            Serial.print(" "); //pad single digit with space
            Serial.print(data_val, HEX);
          }
          Serial.print(" "); //space between entries
        }
       /*
        // write all channel values to serial
        for (uint16_t i = 1; i <=512; i++) {
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        */
        
        Serial.println(" ");
        
        //Output color commands to LED strips
        LED0.setAllLedsColorData(data[dmxStartAddr+0], data[dmxStartAddr+1], data[dmxStartAddr+2]);
        LED0.setBrightness(data[dmxStartAddr+3]);
        LED0.show();
        LED1.setAllLedsColorData(data[dmxStartAddr+4], data[dmxStartAddr+5], data[dmxStartAddr+6]);
        LED1.setBrightness(data[dmxStartAddr+7]);
        LED1.show();
        LED2.setAllLedsColorData(data[dmxStartAddr+8], data[dmxStartAddr+9], data[dmxStartAddr+10]);
        LED2.setBrightness(data[dmxStartAddr+11]);
        LED2.show();

        //Output commands to relays
        if(data[dmxStartAddr+12]>127)
          quadRelay.turnRelayOn(1); 
        else
          quadRelay.turnRelayOff(1);

        if(data[dmxStartAddr+13]>127)
          quadRelay.turnRelayOn(2); 
        else
          quadRelay.turnRelayOff(2);

        if(data[dmxStartAddr+14]>127)
          quadRelay.turnRelayOn(3); 
        else
          quadRelay.turnRelayOff(3);

        if(data[dmxStartAddr+15]>127)
          quadRelay.turnRelayOn(4); 
        else
          quadRelay.turnRelayOff(4);

        // ==== Output to Display ==== //
       
        // Data has been received, print out channel values to display
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("START: ");
        display.println(dmxStartAddr, DEC);
        for (uint16_t i = 0; i < 16; i++) {
          data_val = data[dmxStartAddr+i];
          if (data_val > 0xF){
            display.print(data_val, HEX);
          } else {
            display.print(" "); //pad single digit with space
            display.print(data_val, HEX);
          }
          display.print(" "); //space between entries
          if (i%4>=3) {display.println(" ");}
        }
        display.display(); // actually display all of the above

       /*
        //For each page. 8 channels per line. 8 lines per page.
        //for (page=0; page<=7; page++){
          display.clearDisplay();
          display.setCursor(105,0);
          display.print(page%8*64, DEC);
          display.setCursor(0,0);
          for (page_chan=0; page_chan <= 63; page_chan++){
            data_val = data[page%8*64+1];
            //If single digit, pad with space
            if (data_val > 0xF){
              display.print(data_val, HEX);
            } else {
              display.print(" ");
              display.print(data_val, HEX);
            }
            
            if (page_chan%8>=7) {display.println(" ");}
          }
          display.display(); // actually display all of the above
          page++;
          */
        //}
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

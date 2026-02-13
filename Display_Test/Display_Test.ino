#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

uint16_t page_chan=0;
uint16_t page=0;

void setup() {
  Serial.begin(115200);

  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Clear the buffer.
  display.clearDisplay();
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  Serial.println(page_chan);

}

void loop() {
  //For each page. 8 channels per line. 8 lines per page.
  for (page=0; page<=7; page++){
    display.clearDisplay();
    display.setCursor(105,0);
    display.print(page*64, DEC);
    display.setCursor(0,0);
    for (page_chan=0; page_chan <= 63; page_chan++){
      Serial.print(page*64+page_chan);
      display.print(page*64+page_chan, HEX);
      if (page_chan%8>=7) {
        display.println(" ");
        Serial.println(" ");
      }
    }
    display.display(); // actually display all of the above
    yield();
    delay(1000);
  }

  
}

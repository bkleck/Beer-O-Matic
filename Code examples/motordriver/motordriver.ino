/*
 Standard configurations for the prodriver library

  ARDUINO --> PRODRIVER
  D8 --> STBY
  D7 --> EN
  D6 --> MODE0
  D5 --> MODE1
  D4 --> MODE2
  D3 --> MODE3
  D2 --> ERR

   - if need to change, use myProDriver.settings.standbyPin = A0;
   - follow example 10 in library
 */

#include "SparkFun_ProDriver_TC78H670FTG_Arduino_Library.h"
#include <ezButton.h>
PRODRIVER myProDriver; //Create instance of driver

const int limitSwitchPin = 12;
bool limit;

void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);
  myProDriver.begin();
}

void loop() {
  limit = digitalRead(limitSwitchPin);
  if (limit == LOW) {
    Serial.println("reached limit");
    myProDriver.step(0, 0);
    myProDriver.disable();
  }

  else {
    myProDriver.enable();
    myProDriver.step(200, 0);
  }
}

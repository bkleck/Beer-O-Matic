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

const int leftLimit = 12;
const int rightLimit = 11;
const int IR1 = A0;
const int button1 = A1;

bool limit;
int dir = 0;
int init_light_reading;
int light_reading;

char currentPos;
char targetPos;
const char *sequence[5] = {"LEFT", "1", "2", "3", "RIGHT"};

/* function to call when the circuit is first turned on
 *  this will turn the motor anticlockwise towards the left limit
 *  stops when it hits the left limit
 */
void Startup() {
  while (ReadPin(leftLimit)) {
    myProDriver.step(1, dir);
  }

  Serial.println("reached left limit");
  currentPos = "LEFT";
  dir = dir + 1;
  delay(3000);
}


// function to read digital pins, returning 1 or 0
bool ReadPin(int pinNumber) {
  bool result = digitalRead(pinNumber);
  return result;
}

char ReadTarget() {
  if ReadPin(A1) {
    targetPos = "1";
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  myProDriver.begin();
  init_light_reading = analogRead(IR1);
  Startup();
}

void loop() {
  if (ReadPin(rightLimit)) {
    myProDriver.step(1, dir);

    light_reading = analogRead(IR1);
    if (light_reading <= init_light_reading - 100) {
      Serial.println("Something fat passed by");
    }
  }

  else {
    Serial.println("reached right limit");
  }
  
}

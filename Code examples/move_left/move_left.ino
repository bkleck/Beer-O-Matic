#include "SparkFun_ProDriver_TC78H670FTG_Arduino_Library.h"
PRODRIVER myProDriver; //Create instance of driver

const int leftLimit = 12;
const int rightLimit = 11;
const int IR1 = A0;
const int IR2 = A3;
int dir = 0;
int init_light_reading;

// function to read digital pins, returning 1 or 0
bool ReadPin(int pinNumber) {
  bool result = digitalRead(pinNumber);
  return result;
}

void Startup() {
  while (ReadPin(leftLimit)) {
    myProDriver.step(1, dir);
  }

  Serial.println("reached left limit");
  dir = dir + 1;
  delay(3000);
}

bool ReadIRSensor(int IR_pin) {
  int light_reading = analogRead(IR_pin);
  if (light_reading <= init_light_reading - 25) {
    Serial.println("Something fat passed by: " + IR_pin);
    return 0;
  }
  else {
    return 1;
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
  Serial.println((String)"IR1: " + ReadIRSensor(IR1));
  Serial.println((String)"IR2: " + ReadIRSensor(IR2));
  if (ReadPin(rightLimit)) {
    if (ReadIRSensor(IR1) == HIGH) {
      myProDriver.step(1, dir);
    }
  }
  
}

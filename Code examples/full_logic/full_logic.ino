#include "SparkFun_ProDriver_TC78H670FTG_Arduino_Library.h"
#include <ezButton.h>
PRODRIVER myProDriver; //Create instance of driver

const int leftLimit = 12;
const int rightLimit = 11;
const int IR1 = A0;
const int Button1 = A1;

bool limit;
int dir = 0;
int init_light_reading;

String currentPos;
String targetPos;
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

// function to set the target position whenever a button is read HIGH
char ReadTarget() {
  if (ReadPin(Button1) == LOW) {
    targetPos = "1";
  }
}

// function to set the current position whenever a limit switch or IR sensor is activated
char ReadCurrent() {
  if (ReadPin(leftLimit) == LOW) {
    currentPos = "LEFT";
  }
  
  else if (ReadPin(rightLimit) == LOW) {
    currentPos = "RIGHT";
  }

  else if (ReadIRSensor(IR1) == LOW) {
    currentPos = "1";
  }
}


bool ReadIRSensor(int IR_pin) {
  int light_reading = analogRead(IR1);
  if (light_reading <= init_light_reading - 100) {
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
  pinMode(A1, INPUT_PULLUP);
  myProDriver.begin();
  init_light_reading = analogRead(IR1);
  // Startup();
}

void loop() {
  Serial.println(ReadPin(Button1));

  ReadTarget();
  ReadCurrent();
  Serial.print("current position: " ); 
  Serial.println(currentPos);
  Serial.print("target position: "); 
  Serial.println(targetPos);
  delay(1000);
  if (targetPos) {
    
  }

  

  else {
    Serial.println("reached right limit");
  }
  
}

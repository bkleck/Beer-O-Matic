#include "SparkFun_ProDriver_TC78H670FTG_Arduino_Library.h"
#include <ezButton.h>

PRODRIVER myProDriver; //Create instance of driver

const int leftLimit = 12;
const int rightLimit = 11;
const int IR1 = A0;
const int IR2 = A3;
const int Button1 = A1;
const int Button2 = A2;
int queueSize = 0;

bool limit;
int dir = 0;
int init_light_reading;

int currentPos;
int targetPos;
const int sequence[5] = {0, 1, 2, 3, 4};
int targetQueue[3];
int currentIndex;
int targetIndex;

/* function to call when the circuit is first turned on
 *  this will turn the motor anticlockwise towards the left limit
 *  stops when it hits the left limit
 */
void Startup() {
  while (ReadPin(leftLimit)) {
    myProDriver.step(200, dir);
  }

  Serial.println("reached left limit");
  currentPos = 0;
  dir = dir + 1;
  delay(3000);
}


// function to read digital pins, returning 1 or 0
bool ReadPin(int pinNumber) {
  bool result = digitalRead(pinNumber);
  return result;
}


// function to set the target position whenever a button is read LOW due to PULLUP mode
char ReadTarget() {
  if (ReadPin(Button1) == LOW) {
    bool elementInside = false;
    for (int i = 0; i < 3; i++) {
      if (targetQueue[i] == 1) {
        elementInside = true;
      }
    }
    if (elementInside == false) {
      targetQueue[queueSize++] = 1;
    }
}

  else if (ReadPin(Button2) == LOW) {
    bool elementInside = false;
    for (int i = 0; i < 3; i++) {
      if (targetQueue[i] == 2) {
        elementInside = true;
      }
    }
    if (elementInside == false) {
      targetQueue[queueSize++] = 2;
    }
}
}


// function to set the current position whenever a limit switch or IR sensor is activated
// as the pins are on PULLUP mode, they are activated when read as LOW
char ReadCurrent() {
  if (ReadPin(leftLimit) == LOW) {
    currentPos = 0;
  }
  
  else if (ReadPin(rightLimit) == LOW) {
    currentPos = 4;
  }

  else if (ReadIRSensor(IR1) == LOW) {
    currentPos = 1;
  }

  else if (ReadIRSensor(IR2) == LOW) {
    currentPos = 2;
  }
}


/* Function to read the IR sensors
 * IR sensors are connected to analog pins giving raw integer values
 * I defined HIGH as the initial reading of IR1 at startup
 * and the current reading as LOW when current reading goes below 100 of initial reading
 */
bool ReadIRSensor(int IR_pin) {
  int light_reading = analogRead(IR_pin);
  if (light_reading <= init_light_reading - 100) {
    Serial.println("Something fat passed by: " + IR_pin);
    return 0;
  }
  else {
    return 1;
  }
}


/*
 * function to move to target position 
 * get the index of the target and current position within the sequence array
 * if the target index is greater, means it should move to the right
 * if the target index is smaller, means it should move left
 * 
 * if the target position is equal to current position, the motor will not move
 * if not it will move based on the direction given above
 */
void MoveToTarget() {
  if (targetPos != NULL) {
    for (int i=0; i < 5; i++) {
      if (targetPos == sequence[i]) {
        targetIndex = i;
      }
      else if (currentPos == sequence[i]) {
        currentIndex = i;
      }
    }

    if (targetIndex > currentIndex) {
      dir = 1;
    }

    else if (targetIndex < currentIndex) {
      dir = 0;
    }

    if (targetPos == currentPos) {
      Serial.println("reached target");
    }
    else {
      myProDriver.step(1, dir);
    }
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
//  myProDriver.begin();
//  init_light_reading = analogRead(IR1);
//  Startup();
}

void loop() {
  Serial.println(sizeof(targetQueue));
  for (int i=0; i<3; i++) {
    Serial.println(targetQueue[i]);
  }

  ReadTarget();
  ReadCurrent();
  Serial.print("current position: " ); 
  Serial.println(currentPos);
  Serial.print("target position: "); 
  Serial.println(targetPos);

  MoveToTarget();
  delay(500);
}

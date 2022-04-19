#include "SparkFun_ProDriver_TC78H670FTG_Arduino_Library.h"
#include <ezButton.h>

PRODRIVER myProDriver; //Create instance of driver

// for main mechanism
const int leftLimit = 12;
const int rightLimit = 11;
const int IR1 = A0;
const int IR2 = A3;
const int IR3 = A5;
const int Button1 = 50;
const int Button2 = 51;
const int Button3 = 52;

// for pump
#define trigPin 27
#define echoPin 33
#define relayPin 22
float duration, distance, time_required;
float final_distance = 5; // 5(ultra to cup top) + 2 (buffer)
float flow_rate = 22.2; //cm^3 per second (to be determined)
float surface_area = 43; // (to be determined)
float height_rate = flow_rate/surface_area; //rate of height increase
int count = 0; //counter for millis
float max_time = 20; //maximum pump run time
//int buffer_time = 2;// time require for water to reach tip of tube from tank(seconds)
long previousMillis = 0;


bool limit;

int dir = 0;
int init_light_reading;

int currentPos;
const int sequence[5] = {0, 1, 2, 3, 4};
int targetQueue[3] = {0, 0, 0};
int currentIndex;
int targetIndex;

/* function to call when the circuit is first turned on
 *  this will turn the motor anticlockwise towards the left limit
 *  stops when it hits the left limit
 */
void Startup() {
  while (ReadPin(leftLimit)) {
    myProDriver.step(1, dir);
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



// function to set the target position to the queue whenever a button is read LOW due to PULLUP mode
char ReadTarget() {

  int queueSize = 0;
  // reset queueSize based on position of first zero
  // this queueSize will be used as the index to add the latest element to the list
  for (int i = 0; i < 5; i++) {
    if (targetQueue[i] == 0) {
      queueSize = i;
      break;
    }
  }
  // Serial.println((String) "queue size:" + queueSize);
  
  if (ReadPin(Button1) == LOW) {
    Serial.println("button 1 pressed");
    bool elementInside1 = false;
    // if any element inside the queue is I, this means that the element is already inside
    for (int i = 0; i < 4; i++) {
      if (targetQueue[i] == 1) {
        elementInside1 = true;
      }
    }
    // only add the element to the queue if it was not inside already
    if (elementInside1 == false) {
      targetQueue[queueSize++] = 1;
    }
}

  if (ReadPin(Button2) == LOW) {
    Serial.println("button 2 pressed");
    bool elementInside2 = false;
    for (int i = 0; i < 4; i++) {
      if (targetQueue[i] == 2) {
        elementInside2 = true;
      }
    }
    if (elementInside2 == false) {
      targetQueue[queueSize++] = 2;
    }
}

  if (ReadPin(Button3) == LOW) {
  Serial.println("button 3 pressed");
    bool elementInside3 = false;
    for (int i = 0; i < 4; i++) {
      if (targetQueue[i] == 3) {
        elementInside3 = true;
      }
    }
    if (elementInside3 == false) {
      targetQueue[queueSize++] = 3;
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
  
  else if (ReadIRSensor(IR3) == LOW) {
    currentPos = 3;
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
  int targetPos = targetQueue[0];
  Serial.println((String) "Target position: " + targetPos);
  if ((targetPos != NULL) & (targetPos != 0)) {
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
      // once reach the position, dispense liquid
      measure_dispense();
      
      // once reached, remove the first element from the array
      for (int i=1; i<3; i++) {
        targetQueue[i-1] = targetQueue[i];
      }
    }

    // reverses the directions if it hits the left or right limit
    // if its at the left limit, it should move right
    else if (currentPos == 0) {
      dir = 1;
      myProDriver.step(3, dir);
    }

    // if its at the right limit, it should move left
    else if (currentPos == 4) {
      dir = 0;
      myProDriver.step(3, dir);
    }

    else {
      myProDriver.step(3, dir);
    }
  }
}


/* function to calculate distance between cup and pump
 *  this is done by calculating the time for ultrasound wave to reflect
 *  and divide it by the speed of sound
 */
void calc_distance (){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.println(distance);
}

/* function to dispense liquid from the pump after distance has been calculated
 * make use of count to ensure it only dispenses once
 */
void measure_dispense() {
  calc_distance();
  Serial.println(distance);
  time_required = (distance-final_distance)/height_rate;
  if (time_required > max_time){
    time_required = max_time;
  }
  Serial.println(time_required);

  long currentMillis = millis();
  count = 0;
  previousMillis = currentMillis;
  while (currentMillis - previousMillis < time_required*1000) {
    
    if (count == 0){
      myProDriver.disable();
      digitalWrite(relayPin,HIGH);
      previousMillis = currentMillis;
      count = 1;
    }
    myProDriver.disable();
    digitalWrite(relayPin,HIGH);
    currentMillis = millis();
    // ReadTarget();
    }

  digitalWrite(relayPin,LOW);
  myProDriver.enable();

}


void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  
  pinMode(50, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);
  pinMode(52, INPUT_PULLUP);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  myProDriver.begin();
  // get initial reading for comparison when motor moves past
  init_light_reading = analogRead(IR1);
  Startup();
}

void loop() {
  Serial.println("Target Queue Elements:");
  for (int i=0; i<3; i++) {
    Serial.println(targetQueue[i]);
  }

  ReadTarget();
  ReadCurrent();
  Serial.println((String)"current position: " + currentPos); 
  MoveToTarget();
}

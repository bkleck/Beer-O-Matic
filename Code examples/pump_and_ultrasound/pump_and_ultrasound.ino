#define trigPin 32
#define echoPin 14
#define relayPin 25
float duration, distance, time_required;
int offset_distance = 2; // distance from tube tip to cup tip (cm)
int edge_distance = 10; //distance from top of the cup to final height (cm)
int flow_rate = 5; //litres per second (to be determined)
int surface_area; // (to be determined)
int height_rate = flow_rate/surface_area; //rate of height increase
int buffer_time = 2;// time require for water to reach tip of tube from tank(seconds)
long previousMillis = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);

}

void loop() {
  calc_distance();
  time_required = (distance-offset_distance-edge_distance)/height_rate;
  unsigned long currentMillis = millis();
  // Keeps the pump running until fill time is reached
  while(currentMillis - previousMillis < time_required+ buffer_time){
    digitalWrite(relayPin,HIGH);// relayPin controls whether pump is ON/OFF
    previousMillis = currentMillis;}
    
  digitalWrite(relayPin,LOW);
}

//distance from ultrasound sensor to level of water in the cup
void calc_distance (){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
}

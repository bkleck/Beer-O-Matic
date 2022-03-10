#define trigPin 27
#define echoPin 33
#define relayPin 12
float duration, distance, time_required;
int final_distance = 2; //TBD
int flow_rate = 5; //litres per second (to be determined)
int surface_area = 4; // (to be determined)
int height_rate = flow_rate/surface_area; //rate of height increase
int count = 0; //counter for millis
//int buffer_time = 2;// time require for water to reach tip of tube from tank(seconds)
long previousMillis = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
}

void loop(){
  measure_dispense();)

void measure_dispense() {
  // put your main code here, to run repeatedly:
  calc_distance();
  Serial.println(distance);
  time_required = (distance-final_distance)/height_rate;
  Serial.println(time_required);

  long currentMillis = millis();
  count = 0;
  previousMillis = currentMillis;
  while (currentMillis - previousMillis < time_required*1000) {
    if (count == 0){
      digitalWrite(relayPin,HIGH);
      previousMillis = currentMillis;
      count = 1;
    }
    digitalWrite(relayPin,HIGH);
    currentMillis = millis();
    }
  
  digitalWrite(relayPin,LOW);

}

void calc_distance (){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
}

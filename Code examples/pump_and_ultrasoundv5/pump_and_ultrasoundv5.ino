#define trigPin 27
#define echoPin 33
#define relayPin 22
float duration, distance, time_required;
float final_distance = 7; // 5(ultra to cup top) + 2 (buffer)
float flow_rate = 22.2; //cm^3 per second (to be determined)
float surface_area = 43; // (to be determined)
float height_rate = flow_rate/surface_area; //rate of height increase
int count = 0; //counter for millis
float max_time = 20; //maximum pump run time
//int buffer_time = 2;// time require for water to reach tip of tube from tank(seconds)
long previousMillis = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  measure_dispense();
}

void loop(){
  // measure_dispense();
}

void measure_dispense() {
  // put your main code here, to run repeatedly:
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

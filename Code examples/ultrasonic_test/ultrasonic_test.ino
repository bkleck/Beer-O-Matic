const int trigPin = 5;
const int echoPin = 18;

const float speedOfSound = 0.0343; // this is in cm/microsecond
long duration;
float distance;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  Serial.print(duration);

  // Calculate the distance
  distance = duration * speedOfSound/2;

  Serial.print("Distance (cm): ");
  Serial.println(distance);
  delay(1000);
}

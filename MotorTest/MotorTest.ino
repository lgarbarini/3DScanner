

#define step0 10
#define step1 5
#define M0_MS1 7
#define M0_MS2 8
#define M0_MS3 9
#define M1_MS1 A1
#define M1_MS2 A2
#define M1_MS3 A3
#define DIR0  11
#define DIR1  2
#define EN0   6
#define EN1   A0
#define TRIG  3
#define ECHO  4

long duration;



void setup() {
  pinMode(step0, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(M0_MS1, OUTPUT);
  pinMode(M0_MS2, OUTPUT);
  pinMode(M0_MS3, OUTPUT);
  pinMode(M1_MS1, OUTPUT);
  pinMode(M1_MS2, OUTPUT);
  pinMode(M1_MS3, OUTPUT);
  pinMode(DIR0, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(EN0, OUTPUT);
  pinMode(EN1, OUTPUT);




  
  //disable motors while we configure
  digitalWrite(EN0, HIGH);
  digitalWrite(EN1, HIGH);

  digitalWrite(step0, LOW);
  digitalWrite(step1, LOW);

  //set directions forward
  digitalWrite(DIR0, HIGH);
  digitalWrite(DIR1, HIGH);

  
  //make motors use 1/16th microsteps
  digitalWrite(M0_MS1, HIGH);
  digitalWrite(M0_MS2, LOW);
  digitalWrite(M0_MS3, LOW);
  digitalWrite(M1_MS1, HIGH);
  digitalWrite(M1_MS2, LOW);
  digitalWrite(M1_MS3, LOW);
  
  //enable motors
  digitalWrite(EN0, LOW);
  digitalWrite(EN1, LOW);  

  //Ultrasound
  pinMode(TRIG, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO, INPUT); // Sets the echoPin as an Input
  
  delay(2000);
  Serial.begin(250000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.read() != -1) {
    digitalWrite(EN0, LOW);
    digitalWrite(EN1, LOW); 
    for (int i = 0; i < 100; i++) {
      getLine(100);
      resetLine(100);
      Serial.print('\n');
      armStep();
    }
  Serial.write("hello");
  reset();
  }
  digitalWrite(EN0, HIGH);
  digitalWrite(EN1, HIGH); 
}


void armStep() {
  for (int i = 0; i < 1; i++) {
    digitalWrite(step0, HIGH);
    delayMicroseconds(500);
    digitalWrite(step0, LOW);
    delayMicroseconds(500);
  }
}

void getLine(int numLines) {
  for (int j = 0; j < numLines; j++) {
    senseStep();
    Serial.print(getDist());
    Serial.print(',');
  }
}

void resetLine(int numLines) {
  digitalWrite(DIR1, LOW);
  for (int j = 0; j < numLines; j++) {
    senseStep();
  }
  digitalWrite(DIR1, HIGH);
}

void senseStep() {
  digitalWrite(step1, HIGH);
  delayMicroseconds(500);
  digitalWrite(step1, LOW);
  delayMicroseconds(500);
}

void reset(){
  digitalWrite(DIR0, LOW);
  for (int i = 0; i < 100; i++) {
    armStep();
  }
  digitalWrite(DIR0, HIGH);
}

int getDist() {
  int distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO, HIGH, 13000);
  // Calculating the distance
  distance= duration*0.034/2;
  if (duration ==0) distance = 255;
  return distance;
}



#ifndef WIRE_H
#include <Wire.h>
#endif

#ifndef VL53L0X_h
#include "VL53L0X.h"
#endif

//use #defines to configure LIDAR (see library for more details)
#define LIDAR
#define HIGH_SPEED
//#define ULTRASOUND
//#define LONGRANGE
//#define HIGH_ACCURACY
#if defined LIDAR

VL53L0X sensor;

#endif

//defind delays for step pulses
#define step_delay 2000
#define step_delay_high 500

//define pins
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
  
  //setup pinmodes for motor controllers
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

  
  //make motors use 1/2th microsteps
  digitalWrite(M0_MS1, HIGH);
  digitalWrite(M0_MS2, LOW);
  digitalWrite(M0_MS3, LOW);
  digitalWrite(M1_MS1, HIGH);
  digitalWrite(M1_MS2, LOW);
  digitalWrite(M1_MS3, LOW);
  
  //enable motors
  digitalWrite(EN0, LOW);
  digitalWrite(EN1, LOW);  
#if defined ULTRASOUND
  //Ultrasound
  pinMode(TRIG, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO, INPUT); // Sets the echoPin as an Input
#endif

//setup lidar
#if defined LIDAR

  Wire.begin();

  sensor.init();
  sensor.setTimeout(1000);

  #if defined LONG_RANGE
    // lower the return signal rate limit (default is 0.25 MCPS)
    sensor.setSignalRateLimit(0.1);
    // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  #endif
  
  #if defined HIGH_SPEED
    // reduce timing budget to 20 ms (default is about 33 ms)
    sensor.setMeasurementTimingBudget(20000);
  #elif defined HIGH_ACCURACY
    // increase timing budget to 200 ms
    sensor.setMeasurementTimingBudget(200000);
  #endif
#endif
  delay(2000);
  Serial.begin(250000);
}

void loop() {
  //wait for character on serial port before starting scan
  if (Serial.read() != -1) {
    //enable motors
    digitalWrite(EN0, LOW);
    digitalWrite(EN1, LOW); 
    for (int i = 0; i < 100; i++) {
      //step 100 forward, 100 back
      getLine(100);
      resetLine(100);
      Serial.print('\n');
      delay(2);
      //move arm
      armStep();
      delay(5);
    }
  Serial.println("hello");
  //return to original position
  reset();
  }
  //wait for motor to stop
  delay(1000);
  //disable motor drivers
  digitalWrite(EN0, HIGH);
  digitalWrite(EN1, HIGH); 
}


void armStep() {
  digitalWrite(step0, HIGH);
  delayMicroseconds(step_delay_high);
  digitalWrite(step0, LOW);
  delayMicroseconds(step_delay);
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
  delayMicroseconds(step_delay);
  for (int j = 0; j < numLines; j++) {
    senseStep();
  }
  delayMicroseconds(step_delay);
  digitalWrite(DIR1, HIGH);
}

void senseStep() {
  digitalWrite(step1, HIGH);
  delayMicroseconds(step_delay_high);
  digitalWrite(step1, LOW);
  delayMicroseconds(step_delay);
}

void reset(){
  digitalWrite(DIR0, LOW);
  for (int i = 0; i < 100; i++) {
    armStep();
  }
  digitalWrite(DIR0, HIGH);
}

int getDist() {
  #if defined ULTRASOUND
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
  #endif
  #if defined LIDAR

  return sensor.readRangeSingleMillimeters();
  
  #endif
}



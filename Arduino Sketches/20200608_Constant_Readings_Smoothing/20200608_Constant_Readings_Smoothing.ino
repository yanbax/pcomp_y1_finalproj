//Modified version of the Arduino tutorial 'Smoothing'
#include <Wire.h>;
#include "SparkFun_MMA8452Q.h";

MMA8452Q accel;

const int numReadings = 100;

int FSR_readings[numReadings];      // the readings from the analog input
int PHOTO_readings[numReadings];
int ORIEN_readings[numReadings];
int FSR_readIndex = 0;              // the index of the current reading
int PHOTO_readIndex = 0;
int ORIEN_readIndex = 0;
int FSR_total = 0;                  // the running total
int PHOTO_total = 0;
int ORIEN_total;
int FSR_average = 0;                // the average
int PHOTO_average = 0;
int ORIEN_average = 0;

int FSR_pin = A1;
int PHOTO_pin = A0;

void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);
  // initialize all the readings to 0:
  for (int FSR_Reading = 0; FSR_Reading < numReadings; FSR_Reading++) {
    FSR_readings[FSR_Reading] = 0;
  }
  for (int PHOTO_Reading = 0; PHOTO_Reading < numReadings; PHOTO_Reading++) {
    PHOTO_readings[PHOTO_Reading] = 0;
  }
  for (int ORIEN_Reading = 0; ORIEN_Reading < numReadings; ORIEN_Reading++) {
    ORIEN_readings[ORIEN_Reading] = 0;
  }

  Wire.begin();
  accel.init();
}

void loop() {
  int X = analogRead(FSR_pin);
  int Y = analogRead(PHOTO_pin);
  int Z = accel.readPL();

  delay(1000);

  int X2 = analogRead(FSR_pin);
  int Y2 = analogRead(PHOTO_pin);
  int Z2 = accel.readPL();

  if (X2 > X) {
    // subtract the last reading:
    FSR_total = FSR_total - FSR_readings[FSR_readIndex];
    // read from the sensor:
    FSR_readings[FSR_readIndex] = analogRead(FSR_pin);
    // add the reading to the total:
    FSR_total = FSR_total + FSR_readings[FSR_readIndex];
    // advance to the next position in the array:
    FSR_readIndex = FSR_readIndex + 1;
  
    // if we're at the end of the array...
    if (FSR_readIndex >= numReadings) {
      // ...wrap around to the beginning:
      FSR_readIndex = 0;
    }
  
    // calculate the average:
    FSR_average = FSR_total / numReadings;
    // send it to the computer as ASCII digits
    Serial.println(FSR_average);
    Serial.println("It's FSR");
    delay(100);        // delay in between reads for stability
  }
  else if (Y2 > Y) {
    // subtract the last reading:
    PHOTO_total = PHOTO_total - PHOTO_readings[PHOTO_readIndex];
    // read from the sensor:
    PHOTO_readings[PHOTO_readIndex] = analogRead(PHOTO_pin);
    // add the reading to the total:
    PHOTO_total = PHOTO_total + PHOTO_readings[PHOTO_readIndex];
    // advance to the next position in the array:
    PHOTO_readIndex = PHOTO_readIndex + 1;
  
    // if we're at the end of the array...
    if (PHOTO_readIndex >= numReadings) {
      // ...wrap around to the beginning:
      PHOTO_readIndex = 0;
    }
  
    // calculate the average:
    PHOTO_average = PHOTO_total / numReadings;
    // send it to the computer as ASCII digits
    Serial.println(PHOTO_average);
    Serial.println("It's PHOTO");
    delay(100); 
  }
  else if (Z2 > Z) {
    // subtract the last reading:
    ORIEN_total = ORIEN_total - ORIEN_readings[ORIEN_readIndex];
    // read from the sensor:
    ORIEN_readings[ORIEN_readIndex] = accel.readPL();
    // add the reading to the total:
    ORIEN_total = ORIEN_total + ORIEN_readings[ORIEN_readIndex];
    // advance to the next position in the array:
    ORIEN_readIndex = ORIEN_readIndex + 1;
  
    // if we're at the end of the array...
    if (ORIEN_readIndex >= numReadings) {
      // ...wrap around to the beginning:
      ORIEN_readIndex = 0;
    }
  
    // calculate the average:
    ORIEN_average = ORIEN_total / numReadings;
    // send it to the computer as ASCII digits
    Serial.println(ORIEN_average);
    Serial.println("It's ORIEN");
    delay(100); 
  }
}

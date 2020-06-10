//Code including basic setup/loop and a function I created, asking for readings to be taken from 3 sensors when called and assigned to global variables
//The loop function should then print the global variables in question and wait for a while before repeating the process

#include <Wire.h> //using an I2C breakout (accelerometer)
#include "SparkFun_MMA8452Q.h" //accelerometer breakout's library

MMA8452Q accel; //create an instance of this accelerometer

int FSR_pin = A1; //force resistor pin
const int PHOTO_pin = A0; //phototransistor pin

//variables to use to take a base reading, to later measure against for changes
int base_PHOTO = 0; 
int base_FSR = 0;
byte base_ORIEN = 0;

//variable to tell which sensor the user interacted with out of 1 - FSR, 2 - phototransistor, 3 - accelerometer
int option = 0;

//variables 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  accel.init();
}

void baseReading() {
  //Function to get one 'base' reading from the sensors, to later measure changes against
  base_FSR = analogRead(FSR_pin);
  base_PHOTO = analogRead(PHOTO_pin);
  base_ORIEN = accel.readPL();
}

int takeReading() {
  //variables to take readings of changes, to measure against the base variables
  int read_PHOTO = 0;
  int read_FSR = 0;
  byte read_ORIEN = 0;

  
}

void loop() {
  baseReading(); //call my own function to get base readings
  Serial.println(base_FSR);
  Serial.println(base_PHOTO);
  Serial.println(base_ORIEN);
  delay(5000);
}

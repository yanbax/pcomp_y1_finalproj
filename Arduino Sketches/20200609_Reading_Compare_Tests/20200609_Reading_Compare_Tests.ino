//Building on from taking readings sketch
//Includes a variant on the 'Smoothing' Arduino tutorial's sketch

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

//variable to set tolerance for readings
int tolerance = 0;

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

  //Determine a TOLERANCE, outside of which we will assume changes in input are user-generated and not down to environmental fluctuation
  //Tolerance only applies to the method used to read the FSR and PHOTO pins. The readPL method used with the accelerometer does not provide continuous data, only discrete numbers which correspond to e.g. UP or DOWN - hence tolerance is not needed
  //The default tolerance is 50. If either the FSR or PHOTO is at a level which is less than +/-50 away from its max or min readings, we take the difference between it and its max/min as the tolerance instead
  //Exception: Default value for the FSR when not pressed is 0, so we only check if the PHOTO is < 50
  if (base_PHOTO < 50) {
    tolerance = base_PHOTO - 1;
  }
  else if ((max(base_FSR, base_PHOTO)) > (1023 - 50)) {
    tolerance = 1023 - ((max(base_FSR, base_PHOTO)) - 1);
  }
  else {
    tolerance = 50;
  }

  Serial.println("Tolerance is");
  Serial.println(tolerance);
}

int takeReading() {
  //This code ultimately gets DROPPED from the final implementation, as this is the next step on from the MVP handed in. However, this is the beginnings of a more complex implementation for future scaling up
  //variables to take readings of changes, to measure against the base variables
  const int numReadings = 100;
  int readings[numReadings];
  int readIndex = 0;
  int total = 0;
  int average = 0;
  int readPin = 0;

  for (int reading = 0; reading < numReadings; reading++) {
    readings[reading] = 0;
  }

  if (option = 1) {
    readPin = A1;
  }
  else if (option = 2) {
    readPin = A0;
  }
  
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(readPin);
  
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  
  // calculate the average:
  //This next line would need to be revised if implemented, as the numbers are currently way too low to accurately reflect the change - perhaps the number of samples could be knocked down
  average = total / numReadings;
  Serial.println("Option is");
  Serial.println(option);
  Serial.println("Average is");
  Serial.println(average);
  delay(100); 
}

void loop() {
  baseReading(); //call my own function to get base readings
  Serial.println(base_FSR);
  Serial.println(base_PHOTO);
  Serial.println(base_ORIEN);

  delay(900); //This will be replaced with a push button in the final code to add reliablity; it will also be wrapped in a slightly different function
  
  if (analogRead(FSR_pin) > base_FSR + tolerance) {
    option = 1;
    takeReading();
  }
  else if (analogRead(PHOTO_pin) > base_PHOTO + tolerance || analogRead(PHOTO_pin) < base_PHOTO - tolerance) {
    option = 2;
    takeReading();
  }
  else if (accel.readPL() != base_ORIEN) {
    option = 3;
    //Don't need to take a reading to average here - there's only 4 discrete options relevant to us, so just figure out which was chosen
    Serial.println("Option is");
    Serial.println(option);
    if (accel.isRight() == true) {
      Serial.println("Right");
    }
    else if (accel.isLeft() == true) {
      Serial.println("Left");
    }
    else if (accel.isUp() == true) {
      Serial.println("Up");
    }
    else if (accel.isDown() == true) {
      Serial.println("Down");
    }
  }
  else {
    Serial.println("NO CHANGE");
  }
}

#include <Wire.h>
#include "SparkFun_MMA8452Q.h"

MMA8452Q accel;

void setup(){
 Serial.begin(9600); 
 Wire.begin();
 accel.init(); //THIS WAS MISSING, AND FIXED THE PROBLEM
}

void loop(){
  if (accel.available()) {      // Wait for new data from accelerometer
    // Orientation of board (Right, Left, Down, Up);
   Serial.println(accel.readPL());
  }
}

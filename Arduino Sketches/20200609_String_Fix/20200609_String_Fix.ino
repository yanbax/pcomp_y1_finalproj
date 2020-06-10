//This version uses a solution for committing the Strings, which were haemorraghing memory, to character arrays in PROGMEM instead of SRAM
//Solution found via http://www.gammon.com.au/progmem

#include <Wire.h> //using an I2C breakout (accelerometer)
#include "SparkFun_MMA8452Q.h" //accelerometer breakout's library
#include "rgb_lcd.h" //LCD breakout's library

MMA8452Q accel; //create an instance of this accelerometer
rgb_lcd lcd;

//variables to use to take a base reading, to later measure against for changes
int base_PHOTO = 0; 
int base_FSR = 0;
byte base_ORIEN = 0;

//variable to tell which sensor the user interacted with & how; out of 1 - FSR, 2 - phototransistor UP, 3 - phototransistor DOWN, 4 - accelerometer RIGHT, 5 - accelerometer LEFT, 6 - accelerometer UP, 7 - accelerometer DOWN.
int option = 0;

//variable to set tolerance for readings
int tolerance = 0;

//remember which text we displayed last
int game_STEP = 1;

//creating arrays of text to be stored to PROGMEM instead of SRAM
//avoiding the String type in favour of the leaner char array type

const char t1 [] PROGMEM = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
const char t2 [] PROGMEM = "The GROUND to the side of the highway seems slightly spongy, like it you might be able to push your whole arm through it if you tried.";

char text_STEP[184];        // Array to hold the text to print

void setup() {
  Serial.begin(9600);
  Wire.begin();
  accel.init();
  lcd.begin(16, 2);
  const int colorR = 255;
  const int colorG = 0;
  const int colorB = 0;
  lcd.setRGB(colorR, colorG, colorB);
}

void baseReading() {
  //Function to get one 'base' reading from the sensors, to later measure changes against
  base_FSR = analogRead(A1);
  base_PHOTO = analogRead(A0);
  base_ORIEN = accel.readPL();

  //Determine a TOLERANCE, outside of which we will assume changes in input are user-generated and not down to environmental fluctuation
  if (base_PHOTO < 50) {
    tolerance = base_PHOTO - 1;
  }
  else if ((max(base_FSR, base_PHOTO)) > (1023 - 50)) {
    tolerance = 1023 - ((max(base_FSR, base_PHOTO)) - 1);
  }
  else {
    tolerance = 50;
  }
}

int takeReading() {
  if (analogRead(A1) > base_FSR + tolerance) {
    option = 1;
  }
  else if (analogRead(A0) > base_PHOTO + tolerance) {
    option = 2;
  }
  else if (analogRead(A0) < base_PHOTO - tolerance) {
    option = 3;
  }
  else if (accel.readPL() != base_ORIEN) {
    if (accel.isRight() == true) {
      option = 4;
    }
    else if (accel.isLeft() == true) {
      option = 5;
    }
    else if (accel.isUp() == true) {
      option = 6;
    }
    else if (accel.isDown() == true) {
      option = 7;
    }
  }
  else {
    //Recursively call self until something changes due to user input
    takeReading();
  }
  return option;
}

int writeText() {
  //variables to keep track of what's being printed where
  int index_Count = 0;
  int char_Count = 0;
  int line_Ref = 0;
  
  //a string that will change to keep the last line displayed in memory
  String previous_Line = "";

  //read in the data from the PROGMEM, to a new string (
  String used_TEXT = String(text_STEP);

  int text_LENGTH = used_TEXT.length();

  while (index_Count < text_LENGTH) {
    delay(100);
    if (index_Count != 0 && index_Count % 16 == 0) {
      if (line_Ref == 0) {
        //lcd.clear
        line_Ref = 1;
        char_Count = 0;
      }
      else if (line_Ref == 1) {
          delay(1000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(previous_Line);
          previous_Line = "";
          line_Ref = 1;
          char_Count = 0;
        //}
      }
      else {
      }
    }
    lcd.setCursor(char_Count, line_Ref);
    lcd.print(used_TEXT[index_Count]);
    
    previous_Line = previous_Line + used_TEXT[index_Count];
    index_Count++;
    char_Count++;
  }
}

void gameSequence() {
  //Use a truncated version of the gameSequence code to test if this does actually work to knock the memory down
  unsigned int count = 0;
    
  if (game_STEP == 1) {
   text_STEP[0]=0;
   strcpy_P(text_STEP,t1);
   text_STEP[200]=0;
   game_STEP = 2;
   }
  baseReading();
  writeText();
  takeReading();
  lcd.clear();
  
  if (game_STEP == 2) {
      game_STEP = 2;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t2);
      text_STEP[200]=0;
      gameSequence();
    }
}

void loop() {
 gameSequence();
}

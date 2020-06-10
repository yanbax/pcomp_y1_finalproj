//Code implementing the text functions of the game
//Basic test for text wrapping, pushing button to advance

#include <Wire.h>
#include "rgb_lcd.h"

//const int BUTTON_pin = 2;
//int BUTTON_state = 0;

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  //pinMode(BUTTON_pin, INPUT);
}

unsigned int index_Count = 0;
unsigned int char_Count = 0;
int line_Ref = 0;

String text1 = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
int text1_LENGTH = text1.length();

//A string that will change to keep the last line displayed in memory
String previous_Line = "";

void loop() {
  delay(100);
  //BUTTON_state = digitalRead(BUTTON_pin);
  //Serial.println(BUTTON_state);
  previous_Line = previous_Line + text1[index_Count];

  if (index_Count < text1_LENGTH) {
     if (index_Count != 0 && index_Count % 16 == 0) {
      if (line_Ref == 0) {
        //lcd.clear
        line_Ref = 1;
        char_Count = 0;
      }
      else if (line_Ref == 1) {
        //wait for the button to be pressed first
        //while (BUTTON_state == 0) {
          //Do nothing
        //}
        //if (BUTTON_state == 1) {
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
  
    lcd.print(text1[index_Count]);
    index_Count++;
    char_Count++;
  }
}

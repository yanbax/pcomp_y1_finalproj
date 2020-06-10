//This version a draft which uses too many STRINGS to be able to cope. The Ardunio's memory cannot handle them at all.

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

String text_STEP = "";

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

int writeText(String text_STEP) {
  //variables to keep track of what's being printed where
  unsigned int index_Count = 0;
  unsigned int char_Count = 0;
  int line_Ref = 0;
  int text_LENGTH = text_STEP.length();
  
  //A string that will change to keep the last line displayed in memory
  String previous_Line = "";

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
  
    lcd.print(text_STEP[index_Count]);
    previous_Line = previous_Line + text_STEP[index_Count];
    index_Count++;
    char_Count++;
  }
}

void gameSequence() {
  if (game_STEP == 1) {
   text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
  }
  baseReading();
  writeText(text_STEP);
  takeReading();
  
  if (game_STEP == 1) {
    if (option == 1) {
      game_STEP = 2;
      text_STEP = "The GROUND to the side of the highway seems slightly spongy, like it you might be able to push your whole arm through it if you tried.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP = "Using your TORCH, you investigate the fissures in the highway. Beyond some of the larger gaps in the concrete, you think you see deep CAVERNS.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 14;
      text_STEP = "You cover your eyes and squint towards the other end of the HIGHWAY. Against the clear sky, you see the outline of a CHURCH far off.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP = "You look around and notice a HOUSE to your side, recessed back from the road, half covered by a huge clump of weeds.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP = "You crouch down take a closer look at the GRAFFITI.";
      gameSequence();
    }
  }
  else if (game_STEP == 2) {
    game_STEP = 1;
    text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
    gameSequence();
  }
  else if (game_STEP == 3) {
    if (option == 1) {
      game_STEP = 45;
      text_STEP = "You brush the rough tarmac with your hands. PAINT flakes off and is carried away by the breeze.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP = "Using your TORCH, you investigate the fissures in the highway. Beyond some of the larger gaps in the concrete, you think you see deep CAVERNS.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 5;
      text_STEP = "As a cloud passes over the sun, you think you can see wispy VAPOURS rising up from the cracked roadway.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 4;
      text_STEP = "You look to your side and see a large DEVIL, easily 4 feet high, sprayed on the ground. Shaky, underlined writing beneath it says: ‘TRUE’.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP = "You crouch down take a closer look at the GRAFFITI.";
      gameSequence();
    }
  }
  else if (game_STEP == 4) {
    game_STEP = 3;
    text_STEP = "You crouch down take a closer look at the GRAFFITI.";
    gameSequence();
  }
  else if (game_STEP == 5) {
    game_STEP = 3;
    text_STEP = "You crouch down take a closer look at the GRAFFITI.";
    gameSequence();
  } 
  else if (game_STEP == 6) {
    if (option == 1) {
      game_STEP = 8;
      text_STEP = "You touch a what looks like a RAISED FLOWER BED, which has managed to fight off the weeds. The earth is dry and rocky, like it hasn’t been tilled in decades.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP = "Using your TORCH, you investigate the fissures in the highway. Beyond some of the larger gaps in the concrete, you think you see deep CAVERNS.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 7;
      text_STEP = "You cover your eyes. Is the FRONT DOOR open?";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 1;
      text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 9;
      text_STEP = "You are in front of the HOUSE. The pebbledash on its walls is flaking off like a sunburn. You are standing at its North face, before the FRONT DOOR.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP = "You crouch down take a closer look at the GRAFFITI.";
      gameSequence();
    }
  }
  else if (game_STEP == 7) {
    game_STEP = 6;
    text_STEP = "You look around and notice a HOUSE to your side, recessed back from the road, half covered by a huge clump of weeds.";
    gameSequence();
  }
  else if (game_STEP == 8) {
    game_STEP = 6;
    text_STEP = "You look around and notice a HOUSE to your side, recessed back from the road, half covered by a huge clump of weeds.";
    gameSequence();
  }
  else if (game_STEP == 9) {
    if (option == 1) {
      game_STEP = 13;
      text_STEP = "The FRONT DOOR is slightly open. You give it a good PUSH, but it won’t budge. A shadow seems to indicate something behind it.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 12;
      text_STEP = "As the sun breaks out from behind a cloud, you notice something GLINTING from under a layer of grime in the porch.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 10;
      text_STEP = "Covering your eyes to take a closer look, you realise the FRONT DOOR is definitely ajar, and think you catch a quick flit of movement from INSIDE.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 1;
      text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 13;
      text_STEP = "The FRONT DOOR is slightly open. You give it a good PUSH, but it won’t budge. A shadow seems to indicate something behind it.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 11;
      text_STEP = "Bending down, you find a rusted, heavy KEY, half covered by a battered ‘WELCOME’ mat. ";
      gameSequence();
    }
  }      
  else if (game_STEP == 10) {
    game_STEP = 9;
    text_STEP = "You are in front of the HOUSE. The pebbledash on its walls is flaking off like a sunburn. You are standing at its North face, before the FRONT DOOR.";
    gameSequence();
  } 
  else if (game_STEP == 11) {
    game_STEP = 9;
    text_STEP = "You are in front of the HOUSE. The pebbledash on its walls is flaking off like a sunburn. You are standing at its North face, before the FRONT DOOR.";
    gameSequence();
  } 
  else if (game_STEP == 12) {
    game_STEP = 9;
    text_STEP = "You are in front of the HOUSE. The pebbledash on its walls is flaking off like a sunburn. You are standing at its North face, before the FRONT DOOR.";
    gameSequence();
  } 
  else if (game_STEP == 13) {
    game_STEP = 9;
    text_STEP = "You are in front of the HOUSE. The pebbledash on its walls is flaking off like a sunburn. You are standing at its North face, before the FRONT DOOR.";
    gameSequence();
  }
  else if (game_STEP == 14) {
    if (option == 1) {
      game_STEP = 46;
      text_STEP = "You bring your hands in closer and squint harder. You think you see a figure moving near the CHURCH.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP = "Using your TORCH, you investigate the fissures in the highway. Beyond some of the larger gaps in the concrete, you think you see deep CAVERNS.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 46;
      text_STEP = "You bring your hands in closer and squint harder. You think you see a figure moving near the CHURCH.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP = "You look around and notice a HOUSE to your side, recessed back from the road, half covered by a huge clump of weeds.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP = "You crouch down take a closer look at the GRAFFITI.";
      gameSequence();
    }
  }  
  else if (game_STEP == 15) {
    if (option == 1) {
      game_STEP = 16;
      text_STEP = "You push down on the large DOOR HANDLE, but the sound of the oak door hitting against a lock echoes back.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 30;
      text_STEP = "Shining your TORCH into the porch rafters, you can see layers on layers of thick cobwebs.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 17;
      text_STEP = "Blocking out the sun and peering around, it looks like the place is deserted, even though the porch FLOOR was clearly swept recently.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 47;
      text_STEP = "Thick OVERGROWTH stops you from moving any further around the church grounds.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 16;
      text_STEP = "You push down on the large DOOR HANDLE, but the sound of the oak door hitting against a lock echoes back.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 18;
      text_STEP = "You notice a small WINDOW near the ground, like you might find in a BASEMENT. You peek in and, through the dirt, can see the silhouette of someone moving around inside.";
      gameSequence();
    }
  }
  else if (game_STEP == 16) {
    game_STEP = 15;
    text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
    gameSequence();
  } 
  else if (game_STEP == 17) {
    game_STEP = 15;
    text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
    gameSequence();
  } 
  else if (game_STEP == 18) {
    if (option == 1) {
      game_STEP = 21;
      text_STEP = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 19;
      text_STEP = "You move your TORCH to the window to get a better look, but the PERSON notices the beam, sees you and hurriedly exits the BASEMENT. You decide it's best to stop snooping.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 20;
      text_STEP = "Cupping the glass with your hands, you can see an ornate METAL STRUCTURE behind them on the TABLE. They have their back to you, so you can’t be sure, but it looks like they are carefully lighting CANDLES around it.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 15;
      text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 21;
      text_STEP = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
      gameSequence();
    }
  }  
  else if (game_STEP == 19) {
    game_STEP = 15;
    text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
    gameSequence();
  }
  else if (game_STEP == 20) {
    if (option == 1) {
      game_STEP = 21;
      text_STEP = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 19;
      text_STEP = "You move your TORCH to the window to get a better look, but the PERSON notices the beam, sees you and hurriedly exits the BASEMENT. You decide it's best to stop snooping.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 20;
      text_STEP = "Cupping the glass with your hands, you can see an ornate METAL STRUCTURE behind them on the TABLE. They have their back to you, so you can’t be sure, but it looks like they are carefully lighting CANDLES around it.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 15;
      text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 21;
      text_STEP = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
      gameSequence();
    }
  }
  else if (game_STEP == 21) {
    if (option == 1) {
      game_STEP = 25;
      text_STEP = "You gently pick up a CANDLE from the table. Unlit, but still warm. The STRANGER definitely isn’t in here now.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 24;
      text_STEP = "Shining your TORCH around, you find what looks like an EXIT.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 23;
      text_STEP = "Turning off your TORCH, you notice a sliver of light glowing in the corner.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 22;
      text_STEP = "You notice a DOOR up a pair of steps.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 48;
      text_STEP = "The FLOOR of the BASEMENT is bare dirt.You reach out to touch it. Strangely, it feels slightly warm.";
      gameSequence();
    }
  }
  else if (game_STEP == 22) {
    if (option == 1) {
      game_STEP = 27;
      text_STEP = "You push all your weight against the DOOR, but it won’t budge.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 28;
      text_STEP = "Shining your TORCH across the panelled DOOR, you notice cracks running in all directions across the wood. It looks pretty weak.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 29;
      text_STEP = "You can definitely see artificial LIGHT from the gap beneath the DOOR. Is someone behind it?";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 26;
      text_STEP = "You turn the HANDLE of the DOOR, but it’s obviously locked. How did the STRANGER leave?";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 21;
      text_STEP = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 29;
      text_STEP = "You can definitely see artificial LIGHT from the gap beneath the DOOR. Is someone behind it?";
      gameSequence();
    }
  }         
  else if (game_STEP == 23) {
    game_STEP = 22;
    text_STEP = "You notice a DOOR up a pair of steps.";
    gameSequence();
  }
  else if (game_STEP == 24) {
    game_STEP = 22;
    text_STEP = "You notice a DOOR up a pair of steps.";
    gameSequence();
  }
  else if (game_STEP == 25) {
    game_STEP = 21;
    text_STEP = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
    gameSequence();
  }
  else if (game_STEP == 26) {
    game_STEP = 22;
    text_STEP = "You notice a DOOR up a pair of steps.";
    gameSequence();
  }
  else if (game_STEP == 27) {
    game_STEP = 22;
    text_STEP = "You notice a DOOR up a pair of steps.";
    gameSequence();
  }
  else if (game_STEP == 28) {
    game_STEP = 22;
    text_STEP = "You notice a DOOR up a pair of steps.";
    gameSequence();
  }
  else if (game_STEP == 29) {
    game_STEP = 22;
    text_STEP = "You notice a DOOR up a pair of steps.";
    gameSequence();
  }
  else if (game_STEP == 30) {
    game_STEP = 15;
    text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
    gameSequence();
  }
  else if (game_STEP == 31) {
    if (option == 1) {
      game_STEP = 32;
      text_STEP = "You stand in the entrance of the CAVERN, which is curtained by lichen and exposed roots. You brush aside the OVERGROWTH to get a better look inside.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 35;
      text_STEP = "You poke your TORCH around, but it doesn’t reveal much about what’s further in. Should you keep going?";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP = "Without your TORCH, it’s too dark to see more than a foot in front of you. You move forwards carefully.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP = "You look around and notice a HOUSE to your side, recessed back from the road, half covered by a huge clump of weeds.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 36;
      text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
      gameSequence();
    }
  } 
  else if (game_STEP == 32) {
    if (option == 1) {
      game_STEP = 33;
      text_STEP = "Distracted trying to see how deep the depths go, you don’t realise how heavily you’re pulling on the OVERGROWTH to support yourself until it snaps, putting you in the cavern ENTRANCE.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 35;
      text_STEP = "You poke your TORCH around, but it doesn’t reveal much about what’s further in. Should you keep going?";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP = "Without your TORCH, it’s too dark to see more than a foot in front of you. You move forwards carefully.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP = "You look around and notice a HOUSE to your side, recessed back from the road, half covered by a huge clump of weeds.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 36;
      text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
      gameSequence();
    }
  } 
  else if (game_STEP == 33) {
    game_STEP = 36;
    text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
    gameSequence();
  }
  else if (game_STEP == 34) {
    game_STEP = 36;
    text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
    gameSequence();
  }
  else if (game_STEP == 35) {
    game_STEP = 36;
    text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
    gameSequence();
  }
  else if (game_STEP == 36) {
    if (option == 1) {
      game_STEP = 37;
      text_STEP = "Struggling with the uneven ground, you find a BOULDER and lean back on it for support. After a quiet moment, you feel the BOULDER sliding, and hear an ominous rumbling from the rock around you. You make a hurried escape towards daylight and the HIGHWAY.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 40;
      text_STEP = "Shining your TORCH into the far end of the cave, you see what looks a lot like an old MINING TUNNEL.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP = "Without your TORCH, it’s too dark to see more than a foot in front of you. You move forwards carefully.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 44;
      text_STEP = "Turning to your side, you find nothing much except more rock.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 38;
      text_STEP = "Carefully crouching, you feel the GROUND around you. The rock underfoot is slick and cold with DAMP.";
      gameSequence();
    }
  } 
  else if (game_STEP == 37) {
    game_STEP = 1;
    text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
    gameSequence();
  }
  else if (game_STEP == 38) {
    game_STEP = 36;
    text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
    gameSequence();
  }
  else if (game_STEP == 39) {
    game_STEP = 36;
    text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
    gameSequence();
  }
  else if (game_STEP == 40) {
    if (option == 1) {
      game_STEP = 42;
      text_STEP = "Peering further in, trying to decide whether you should move further into the cramped MINING TUNNEL, you lean against a SUPPORTING BEAM. There’s an almightly crack from the rotten wood, and a huge deposit of ROCK falls in front of you. Adrenaline kicks in, and somehow you escape out of the CAVERN.";
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 40;
      text_STEP = "Shining your TORCH into the far end of the cave, you see what looks a lot like an old MINING TUNNEL.";
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP = "Without your TORCH, it’s too dark to see more than a foot in front of you. You move forwards carefully.";
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 44;
      text_STEP = "Turning to your side, you find nothing much except more rock.";
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 41;
      text_STEP = "You enter the MINING TUNNEL, but your BAG accidentally gets caught on a SUPPORTING BEAM. The rotten wood gives way, and there’s a terrible groaning from the ceiling. Adrenaline kicks in, and somehow you escape out of the CAVERN.";
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 43;
      text_STEP = "You notice a strange RESIDUE at your feet and lean over to better investigate. Gingerly pressing your fingers against it, they come back blackened by the RESIDUE. Coal?";
      gameSequence();
    }
  } 
  else if (game_STEP == 41) {
    game_STEP = 1;
    text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
    gameSequence();
  }
  else if (game_STEP == 42) {
    game_STEP = 1;
    text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
    gameSequence();
  }
  else if (game_STEP == 43) {
    game_STEP = 40;
    text_STEP = "Shining your TORCH into the far end of the cave, you see what looks a lot like an old MINING TUNNEL.";
    gameSequence();
  }
  else if (game_STEP == 44) {
    game_STEP = 36;
    text_STEP = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
    gameSequence();
  }
  else if (game_STEP == 45) {
    game_STEP = 3;
    text_STEP = "You crouch down take a closer look at the GRAFFITI.";
    gameSequence();
  }
  else if (game_STEP == 46) {
    game_STEP = 1;
    text_STEP = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
    gameSequence();
  }
  else if (game_STEP == 47) {
    game_STEP = 15;
    text_STEP = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
    gameSequence();
  }
  else if (game_STEP == 48) {
    game_STEP = 21;
    text_STEP = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
    gameSequence();
  }
}

void loop() {
 gameSequence();
}

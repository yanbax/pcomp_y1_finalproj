//yanbax Year 1 pcomp Final Project code
//A text-based game which takes environmental inputs from the user to proceed

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

//inexcusably long text-to-variable dump follows
const char t1 [] PROGMEM = "You are standing on the edge of an abandoned HIGHWAY. The sun beats down, making you uncomfortably hot. The HIGHWAY is littered with GRAFFITI, thick with layers of overlapping paint.";
const char t2 [] PROGMEM = "The GROUND to the side of the highway seems slightly spongy, like it you might be able to push your whole arm through it if you tried.";
const char t3 [] PROGMEM = "You crouch down take a closer look at the GRAFFITI.";
const char t4 [] PROGMEM = "You look to your side and see a large DEVIL, easily 4 feet high, sprayed on the ground. Shaky, underlined writing beneath it says: ‘TRUE’.";
const char t5 [] PROGMEM = "As a cloud passes over the sun, you think you can see wispy VAPOURS rising up from the cracked roadway.";
const char t6 [] PROGMEM = "You look around and notice a HOUSE to your side, recessed back from the road, half covered by a huge clump of weeds.";
const char t7 [] PROGMEM = "You cover your eyes. Is the FRONT DOOR open?";
const char t8 [] PROGMEM = "You touch a what looks like a RAISED FLOWER BED, which has managed to fight off the weeds. The earth is dry and rocky, like it hasn’t been tilled in decades.";
const char t9 [] PROGMEM = "You are in front of the HOUSE. The pebbledash on its walls is flaking off like a sunburn. You are standing at its North face, before the FRONT DOOR.";
const char t10 [] PROGMEM = "Covering your eyes to take a closer look, you realise the FRONT DOOR is definitely ajar, and think you catch a quick flit of movement from INSIDE."; 
const char t11 [] PROGMEM = "Bending down, you find a rusted, heavy KEY, half covered by a battered ‘WELCOME’ mat.";
const char t12 [] PROGMEM = "As the sun breaks out from behind a cloud, you notice something GLINTING from under a layer of grime in the porch.";
const char t13 [] PROGMEM = "The FRONT DOOR is slightly open. You give it a good PUSH, but it won’t budge. A shadow seems to indicate something behind it.";
const char t14 [] PROGMEM = "You cover your eyes and squint towards the other end of the HIGHWAY. Against the clear sky, you see the outline of a CHURCH far off.";
const char t15 [] PROGMEM = "You are standing in the entrance of a fading CHURCH. Its roof is slightly domed, and may have been coloured gold once, a long time ago. The stained glass still looks immaculate.";
const char t16 [] PROGMEM = "You push down on the large DOOR HANDLE, but the sound of the oak door hitting against a lock echoes back.";
const char t17 [] PROGMEM = "Blocking out the sun and peering around, it looks like the place is deserted, even though the porch FLOOR was clearly swept recently.";
const char t18 [] PROGMEM = "You notice a small WINDOW near the ground, like you might find in a BASEMENT. You peek in and, through the dirt, can see the silhouette of someone moving around inside.";
const char t19 [] PROGMEM = "You move your TORCH to the window to get a better look, but the PERSON notices the beam, sees you and hurriedly exits the BASEMENT.";
const char t20 [] PROGMEM = "Cupping the glass with your hands, you can see an ornate METAL STRUCTURE behind them on the TABLE. They have their back to you, so you can’t be sure, but it looks like they are carefully lighting CANDLES around it.";
const char t21 [] PROGMEM = "Leaning forwards to get a better look in, the glass in the WINDOW shatters and you tumble into the BASEMENT. Recovering, you stand up on the dirt floor, now alone inside the CHURCH.";
const char t22 [] PROGMEM = "You notice a DOOR up a pair of steps.";
const char t23 [] PROGMEM = "Turning off your TORCH, you notice a sliver of light glowing in the corner.";
const char t24 [] PROGMEM = "Shining your TORCH around, you find what looks like an EXIT.";
const char t25 [] PROGMEM = "You gently pick up a CANDLE from the table. Unlit, but still warm. The STRANGER definitely isn’t in here now.";
const char t26 [] PROGMEM = "You turn the HANDLE of the DOOR, but it’s obviously locked. How did the STRANGER leave?";
const char t27 [] PROGMEM = "You push all your weight against the DOOR, but it won’t budge.";
const char t28 [] PROGMEM = "Shining your TORCH across the panelled DOOR, you notice cracks running in all directions across the wood. It looks pretty weak.";
const char t29 [] PROGMEM = "You can definitely see artificial LIGHT from the gap beneath the DOOR. Is someone behind it?";
const char t30 [] PROGMEM = "Shining your TORCH into the porch rafters, you can see layers on layers of thick cobwebs.";
const char t31 [] PROGMEM = "Using your TORCH, you investigate the fissures in the highway. Beyond some of the larger gaps in the concrete, you think you see deep CAVERNS.";
const char t32 [] PROGMEM = "You stand in the entrance of the CAVERN, which is curtained by lichen and exposed roots. You brush aside the OVERGROWTH to get a better look inside.";
const char t33 [] PROGMEM = "Distracted trying to see how deep the depths go, you don’t realise how heavily you’re pulling on the OVERGROWTH to support yourself until it snaps, putting you in the cavern ENTRANCE.";
const char t34 [] PROGMEM = "Without your TORCH, it’s too dark to see more than a foot in front of you. You move forwards carefully.";
const char t35 [] PROGMEM = "You poke your TORCH around, but it doesn’t reveal much about what’s further in. Should you keep going?";
const char t36 [] PROGMEM = "You now stand in the centre of the CAVERN. It smells earthy and damp; it seems to stretch further back than you would expect, with the weight of the HIGHWAY only a few feet above you.";
const char t37 [] PROGMEM = "Struggling with the uneven ground, you find a BOULDER and lean back on it for support. After a quiet moment, you feel the BOULDER sliding, and hear an ominous rumbling from the rock around you. You make a hurried escape towards daylight and the HIGHWAY.";
const char t38 [] PROGMEM = "Carefully crouching, you feel the GROUND around you. The rock underfoot is slick and cold with DAMP.";
const char t39 [] PROGMEM = "Cautiously turning around, you slowly reach out and feel a sheer face of rock in the dark.";
const char t40 [] PROGMEM = "Shining your TORCH into the far end of the cave, you see what looks a lot like an old MINING TUNNEL.";
const char t41 [] PROGMEM = "You enter the MINING TUNNEL, but your BAG accidentally gets caught on a SUPPORTING BEAM. The rotten wood gives way, and there’s a terrible groaning from the ceiling. Adrenaline kicks in, and somehow you escape out of the CAVERN.";
const char t42 [] PROGMEM = "Peering further in, trying to decide whether you should move further into the cramped TUNNEL, you lean against a SUPPORTING BEAM. There’s a crack from the rotten wood, a huge deposit of ROCK falls in front of you. Somehow you escape out of the CAVERN.";
const char t43 [] PROGMEM = "You notice a strange RESIDUE at your feet and lean over to better investigate. Gingerly pressing your fingers against it, they come back blackened by the RESIDUE. Coal?";
const char t44 [] PROGMEM = "Turning to your side, you find nothing much except more ROCK.";
const char t45 [] PROGMEM = "You brush the rough tarmac with your hands. PAINT flakes off and is carried away by the breeze.";
const char t46 [] PROGMEM = "You bring your hands in closer and squint harder. You think you see a figure moving near the CHURCH.";
const char t47 [] PROGMEM = "Thick OVERGROWTH stops you from moving any further around the church grounds.";
const char t48 [] PROGMEM = "The FLOOR of the BASEMENT is bare dirt. You reach out to touch it. Strangely, it feels slightly warm.";

char text_STEP[253];        // Array to hold the text to print

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
  option = 0;
  
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
  
  //A string that will change to keep the last line displayed in memory
  String previous_Line = "";

  //read in the data from the PROGMEM, to a new string
  String used_TEXT = String(text_STEP);

  int text_LENGTH = used_TEXT.length();

  while (index_Count < text_LENGTH) {
    delay(100);
    if (index_Count != 0 && index_Count % 16 == 0) {
      if (line_Ref == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(previous_Line);
        previous_Line = "";
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
  //unsigned int count = 0;
  
  baseReading();
  writeText();
  delay(1000);
  takeReading();

  if (option == 0){
    takeReading();
  }
  
  lcd.clear();
  
  if (game_STEP == 1) {
    if (option == 1) {
      game_STEP = 2;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t2);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t31);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 14;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t14);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t6);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t1);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t3);
      text_STEP[253]=0;
      gameSequence();
    }
  }
  else if (game_STEP == 2) {
    game_STEP = 1;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t1);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 3) {
    if (option == 1) {
      game_STEP = 45;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t45);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t31);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 5;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t5);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 4;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t4);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t1);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t3);
      text_STEP[253]=0;
      gameSequence();
    }
  }
  else if (game_STEP == 4) {
    game_STEP = 3;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t3);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 5) {
    game_STEP = 3;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t3);
    text_STEP[253]=0;
    gameSequence();
  } 
  else if (game_STEP == 6) {
    if (option == 1) {
      game_STEP = 8;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t8);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t31);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 7;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t7);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 1;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t1);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 9;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t9);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t3);
      text_STEP[253]=0;
      gameSequence();
    }
  }
  else if (game_STEP == 7) {
    game_STEP = 6;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t7);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 8) {
    game_STEP = 6;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t6);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 9) {
    if (option == 1) {
      game_STEP = 13;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t13);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 12;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t12);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 10;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t10);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 1;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t1);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 13;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t13);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 11;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t11);
      text_STEP[253]=0;
      gameSequence();
    }
  }      
  else if (game_STEP == 10) {
    game_STEP = 9;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t9);
    text_STEP[253]=0;
    gameSequence();
  } 
  else if (game_STEP == 11) {
    game_STEP = 9;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t9);
    text_STEP[253]=0;
    gameSequence();
  } 
  else if (game_STEP == 12) {
    game_STEP = 9;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t9);
    text_STEP[253]=0;
    gameSequence();
  } 
  else if (game_STEP == 13) {
    game_STEP = 9;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t9);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 14) {
    if (option == 1) {
      game_STEP = 46;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t46);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 31;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t31);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 46;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t46);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t6);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t15);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 3;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t3);
      text_STEP[253]=0;
      gameSequence();
    }
  }  
  else if (game_STEP == 15) {
    if (option == 1) {
      game_STEP = 16;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t16);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 30;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t30);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 17;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t17);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 47;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t47);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 16;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t16);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 18;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t18);
      text_STEP[253]=0;
      gameSequence();
    }
  }
  else if (game_STEP == 16) {
    game_STEP = 15;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t15);
    text_STEP[253]=0;
    gameSequence();
  } 
  else if (game_STEP == 17) {
    game_STEP = 15;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t15);
    text_STEP[253]=0;
    gameSequence();
  } 
  else if (game_STEP == 18) {
    if (option == 1) {
      game_STEP = 21;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t21);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 19;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t19);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 20;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t20);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 15;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t15);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t15);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 21;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t21);
      text_STEP[253]=0;
      gameSequence();
    }
  }  
  else if (game_STEP == 19) {
    game_STEP = 15;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t15);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 20) {
    if (option == 1) {
      game_STEP = 21;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t21);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 19;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t19);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 20;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t20);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 15;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t15);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t15);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 21;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t21);
      text_STEP[253]=0;
      gameSequence();
    }
  }
  else if (game_STEP == 21) {
    if (option == 1) {
      game_STEP = 25;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t25);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 24;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t24);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 23;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t23);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 22;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t22);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 15;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t15);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 48;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t48);
      text_STEP[253]=0;
      gameSequence();
    }
  }
  else if (game_STEP == 22) {
    if (option == 1) {
      game_STEP = 27;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t27);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 28;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t28);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 29;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t29);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 26;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t26);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 21;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t21);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 29;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t29);
      text_STEP[253]=0;
      gameSequence();
    }
  }         
  else if (game_STEP == 23) {
    game_STEP = 22;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t22);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 24) {
    game_STEP = 22;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t22);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 25) {
    game_STEP = 21;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t21);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 26) {
    game_STEP = 22;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t22);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 27) {
    game_STEP = 22;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t22);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 28) {
    game_STEP = 22;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t22);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 29) {
    game_STEP = 22;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t22);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 30) {
    game_STEP = 15;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t15);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 31) {
    if (option == 1) {
      game_STEP = 32;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t32);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 35;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t35);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t34);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t6);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t1);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 36;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t36);
      text_STEP[253]=0;
      gameSequence();
    }
  } 
  else if (game_STEP == 32) {
    if (option == 1) {
      game_STEP = 33;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t33);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 35;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t35);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t34);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 6;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t6);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t1);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 36;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t36);
      text_STEP[253]=0;
      gameSequence();
    }
  } 
  else if (game_STEP == 33) {
    game_STEP = 36;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t36);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 34) {
    game_STEP = 36;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t36);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 35) {
    game_STEP = 36;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t36);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 36) {
    if (option == 1) {
      game_STEP = 37;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t37);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 40;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t40);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t34);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 44;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t44);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 1;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t1);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 38;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t38);
      text_STEP[253]=0;
      gameSequence();
    }
  } 
  else if (game_STEP == 37) {
    game_STEP = 1;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t1);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 38) {
    game_STEP = 36;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t36);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 39) {
    game_STEP = 36;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t36);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 40) {
    if (option == 1) {
      game_STEP = 42;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t42);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 2) {
      game_STEP = 40;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t40);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 3) {
      game_STEP = 34;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t34);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 4 || option == 5) {
      game_STEP = 44;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t44);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 6) {
      game_STEP = 41;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t41);
      text_STEP[253]=0;
      gameSequence();
    }
    else if (option == 7) {
      game_STEP = 43;
      text_STEP[0]=0;
      strcpy_P(text_STEP,t43);
      text_STEP[253]=0;
      gameSequence();
    }
  } 
  else if (game_STEP == 41) {
    game_STEP = 1;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t1);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 42) {
    game_STEP = 1;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t1);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 43) {
    game_STEP = 40;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t40);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 44) {
    game_STEP = 36;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t36);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 45) {
    game_STEP = 3;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t3);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 46) {
    game_STEP = 1;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t1);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 47) {
    game_STEP = 15;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t15);
    text_STEP[253]=0;
    gameSequence();
  }
  else if (game_STEP == 48) {
    game_STEP = 21;
    text_STEP[0]=0;
    strcpy_P(text_STEP,t21);
    text_STEP[253]=0;
    gameSequence();
  }
}

void loop() {
 gameSequence();
}

//#include <Adafruit_NeoPixel.h>
#include <Wire.h>

//#define PIN 8

//Adafruit_NeoPixel Neopixel = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);

//unsigned long brightness = 50;
//unsigned char color = 0;

// Include string names of gestures/touches for testing
//#define SKYWRITER_INC_DEBUG_STRINGS
#include "skywriter.h"

unsigned int max_x, max_y, max_z;
unsigned int min_x, min_y, min_z;

unsigned char lastTouch;

void setup() {
  Serial.begin(115200);
  while(!Serial){};
  Serial.println("Hello world!");
 
  
  Skywriter.begin(18, 5);
  Skywriter.onTouch(touch);
  Skywriter.onAirwheel(airwheel);
  Skywriter.onGesture(gesture);
 // Skywriter.onXYZ(xyz);
}

void loop() {
  Skywriter.poll();
}

void xyz(unsigned int x, unsigned int y, unsigned int z){
  if (x < min_x) min_x = x;
  if (y < min_y) min_y = y;
  if (z < min_z) min_z = z;
  if (x > max_x) max_x = x;
  if (y > max_y) max_y = y;
  if (z > max_z) max_z = z;
   
  char buf[64];
//  sprintf(buf, "%05u:%05u:%05u gest:%02u touch:%02u", x, y, z, Skywriter.last_gesture, Skywriter.last_touch);
   sprintf(buf, "%0u:%0u:%0u", x, y, z );
  Serial.println(buf);
}

void gesture(unsigned char type){
//  Serial.println("Got gesture ");
//  Serial.print(type,DEC);
//  Serial.print('\n');


  switch(type)
            {
            //  case 1:
            //    { Serial.println(" GESTURE_GARBAGE "); } break;
              case 2:
                { Serial.println(" WEST_EAST "); } break;
              case 3:
                { Serial.println(" EAST_WEST "); } break;
              case 4:
                { Serial.println(" SOUTH_NORTH "); } break;
              case 5:
                { Serial.println(" NORTH_SOUTH "); } break;
            //  default:
            //    { Serial.println(" not a jesture ");} break;
            }
}

void touch(unsigned char type){
  
if( type != lastTouch )
  {   // new touch

  if(type < 4)
    {
   // Serial.println("Got touched >.< ");
   // Serial.print(type,DEC);
    switch(type)
            {
              case 0:
                { Serial.println(" --DOUBLETAP_CENTER "); } break;
              case 1:
                { Serial.println(" --DOUBLETAP_EAST "); } break;
              case 2:
                { Serial.println(" --DOUBLETAP_NORTH "); } break;
              case 3:
                { Serial.println(" --DOUBLETAP_WEST "); } break;
              case 4:
                { Serial.println(" --DOUBLETAP_SOUTH "); } break;
            //  default:
            //    { Serial.println(" errororo");} break;
            }

    }
      else 
        {
       //   Serial.print(type,DEC);

          switch(type)
            {
              case 5:
                { Serial.println(" SW_TAP_CENTER "); } break;
              case 6:
                { Serial.println(" SW_TAP_EAST "); } break;
              case 7:
                { Serial.println(" SW_TAP_NORTH "); } break;
              case 8:
                { Serial.println(" SW_TAP_WEST "); } break;
              case 9:
                { Serial.println(" SW_TAP_SOUTH "); } break;
              case 10:
                { Serial.println(" SW_TOUCH_CENTER "); } break;
              case 11:
                { Serial.println(" SW_TOUCH_EAST "); } break;
              case 12:
                { Serial.println(" SW_TOUCH_NORTH "); } break;
              case 13:
                { Serial.println(" SW_TOUCH_WEST "); } break;
              case 14:
                { Serial.println(" SW_TOUCH_SOUTH "); } break;
            //  default:
            //    { Serial.println(" errororo");} break;

            }
        }
  
    }

  lastTouch = type;

}

void airwheel(int delta){
//  Serial.println("Got airwheel ");
 // Serial.print(delta);

switch(delta)
            {
              case -11:
                { Serial.println(" clockwise?  "); } break;
              case 11:
                { Serial.println(" counter clock wise ! "); } break;
              default:
                { Serial.println(" no not. ");} break;

            }
}
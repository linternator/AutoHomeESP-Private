#include <Wire.h>
#include <AutoHome.h>
#include "skywriter.h"

AutoHome autohome;

unsigned int max_x, max_y, max_z;
unsigned int min_x, min_y, min_z;

unsigned char lastTouch;
String packet2 = "test";
String packet = "";
int ready = 0;

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  if (!autohome.mqtt_callback(topic, payload, length)) {


    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    Serial.print(packet);

  }
}



void setup() {
  Serial.begin(115200);
  while(!Serial){};
  Serial.println("Hello world!");
 
  // SDA (GPIO21) and SCL (GPIO22).
  Skywriter.begin(18, 5);
  Serial.print("got i2c started");
  Skywriter.onTouch(touch);
  Skywriter.onAirwheel(airwheel);
  Skywriter.onGesture(gesture);
 // Skywriter.onXYZ(xyz);
unsigned int min_x, min_y, min_z;
 Serial.print("started gesuter sensor");

 autohome.begin();

}

void loop() {
  Skywriter.poll();

  if(ready == 1)
    {
      autohome.sendPacket( packet2.c_str() );
    //   autohome.sendPacket( packet2 );

     //String packetOUT = "BUZZER:IS:" + String(m0de) + "," + String(buz) + "," + String(watier);
     //autohome.sendPacket( packetOUT.c_str() );
      ready = 0;
    }


  autohome.loop();

  delay(5);
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
                { Serial.println(" WEST_EAST "); 
                packet2 = "WEST_EAST"; 
                ready = 1; } break;
              case 3:
                { Serial.println(" EAST_WEST ");
                 packet2 = "EAST_WEST"; 
                ready = 1; } break;
              case 4:
                { Serial.println(" SOUTH_NORTH "); 
                packet2 = "SOUTH_NORTH"; 
                ready = 1;} break;
              case 5:
                { Serial.println(" NORTH_SOUTH "); 
                packet2 = "SOUTH_NORTH"; 
                ready = 1;} break;
            //  default:
            //    { Serial.println(" not a jesture ");} break;

          //  autohome.sendPacket( packet.c_str() );

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
                { Serial.println(" --DOUBLETAP_CENTER "); 
                packet2 = "DOUBLETAP_CENTER"; 
                ready = 1; } break;
              case 1:
                { Serial.println(" --DOUBLETAP_EAST ");
                packet2 = "DOUBLETAP_EAST"; 
                ready = 1;} break;
              case 2:
                { Serial.println(" --DOUBLETAP_NORTH "); 
                packet2 = "DOUBLETAP_NORTH"; 
                ready = 1;} break;
              case 3:
                { Serial.println(" --DOUBLETAP_WEST "); 
                packet2 = "DOUBLETAP_WEST"; 
                ready = 1;} break;
              case 4:
                { Serial.println(" --DOUBLETAP_SOUTH "); 
                packet2 = "DOUBLETAP_SOUTH"; 
                ready = 1;} break;
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
                { Serial.println(" SW_TAP_CENTER ");
                packet2 = "SW_TAP_CENTER"; 
                ready = 1; } break;
              case 6:
                { Serial.println(" SW_TAP_EAST "); 
                packet2 = "SW_TAP_EAST"; 
                ready = 1;} break;
              case 7:
                { Serial.println(" SW_TAP_NORTH "); 
                String packet2 = "SW_TAP_NORTH"; 
                ready = 1;} break;
              case 8:
                { Serial.println(" SW_TAP_WEST "); 
                packet2 = "SW_TAP_WEST"; 
                ready = 1;} break;
              case 9:
                { Serial.println(" SW_TAP_SOUTH ");
                packet2 = "SW_TAP_SOUTH"; 
                ready = 1; } break;
              case 10:
                { Serial.println(" SW_TOUCH_CENTER ");
                packet2 = "SW_TOUCH_CENTER"; 
                ready = 1;} break;
              case 11:
                { Serial.println(" SW_TOUCH_EAST "); 
                packet2 = "SW_TOUCH_EAST"; 
                ready = 1;} break;
              case 12:
                { Serial.println(" SW_TOUCH_NORTH ");
                packet2 = "SW_TOUCH_NORTH"; 
                ready = 1;} break;
              case 13:
                { Serial.println(" SW_TOUCH_WEST "); 
                 packet2 = "SW_TOUCH_WEST"; 
                ready = 1; } break;
              case 14:
                { Serial.println(" SW_TOUCH_SOUTH "); 
                packet2 = "SW_TOUCH_SOUTH"; 
                ready = 1; } break;
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
                { Serial.println(" clockwise?  "); 
                packet2 = "clockwise"; 
                ready = 1; } break;
              case 11:
                { Serial.println(" counter clock wise ! ");
                packet2 = "anticlockwise"; 
                ready = 1; } break;
              default:
                { Serial.println(" no not. "); } break;
            }
}
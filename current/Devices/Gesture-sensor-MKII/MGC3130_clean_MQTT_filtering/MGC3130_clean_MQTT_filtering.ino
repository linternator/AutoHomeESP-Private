#include <Wire.h>
#include <AutoHome.h>
#include "skywriter.h"

AutoHome autohome;

unsigned int max_x, max_y, max_z;
unsigned int min_x, min_y, min_z;

unsigned char lastTouch;
String packet = "";

int event_ID = 0;
int important_event_ID = 0;
int event_ID_old = 0;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned long period = 1000;

bool data_available = false;


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
  while (!Serial) {};
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

  startMillis = millis();  //initial start time

  autohome.begin();
}

String translate(int id) {
  switch (id) {
    case 25: return "DOUBLETAP_EAST";
    case 24: return "DOUBLETAP_NORTH";
    case 23: return "DOUBLETAP_WEST";
    case 22: return "DOUBLETAP_SOUTH";
    case 21: return "DOUBLETAP_CENTER";
    case 20: return "SW_TOUCH_EAST";
    case 19: return "SW_TOUCH_NORTH";
    case 18: return "SW_TOUCH_WEST";
    case 17: return "SW_TOUCH_SOUTH";
    case 16: return "SW_TOUCH_CENTER";
    case 15: return "SW_TAP_EAST";
    case 14: return "SW_TAP_NORTH";
    case 13: return "SW_TAP_WEST";
    case 12: return "SW_TAP_SOUTH";
    case 11: return "SW_TAP_CENTER";
    case 10: return "WEST_EAST";
    case 9: return "EAST_WEST";
    case 8: return "SOUTH_NORTH";
    case 7: return "NORTH_SOUTH";
    case 6: return "6";// "clockwise";
    case 5: return "5"; // "anticlockwise";  // could need work.
    case 4: return " 4";
    case 3: return " 3";
    case 2: return " 2";
    case 1: return " 1";
    case 0: return " 0";
    default: " 0";
  }
}

void loop() {
  Skywriter.poll();

  currentMillis = millis();

  if (data_available)  // recived a new gesture.
  {


    if (event_ID > important_event_ID) {
      important_event_ID = event_ID;
    }
    period = currentMillis + 250;
    data_available = false;

    String event_name = translate(event_ID);
    String important_event_name = translate(important_event_ID);
    Serial.print("New event recived: (");
    Serial.print(event_ID);
    Serial.print(")" + event_name + ", Current prioitized event: (");
    Serial.print(important_event_ID);
    Serial.println(")" + important_event_name);


  }



  if (period != 0 && currentMillis >= period)  // if no new inputs and times up output message.
  {
    //   if( event_ID_old != important_event_ID )
    //   {
    String packet2 = translate(important_event_ID);
    Serial.println("Sends message: '"+packet2+"'");
    autohome.sendPacket(packet2.c_str());
    important_event_ID = event_ID_old;
    important_event_ID = 0;
    event_ID = 0;
    period = 0;
    Serial.println("- - - - - - - - Message sent - - - - - - - - ");
    //     }
  }

  autohome.loop();

  delay(5);
}

void gesture(unsigned char type) {
  //  Serial.println("Got gesture ");
  //  Serial.print(type,DEC);
  //  Serial.print('\n');


  switch (type) {
      //  case 1:
      //    { Serial.println(" GESTURE_GARBAGE "); } break;
    case 2:
      {
        Serial.println(" WEST_EAST ");
        event_ID = 10;
        data_available = true;
      }
      break;
    case 3:
      {
        Serial.println(" EAST_WEST ");
        event_ID = 9;
        data_available = true;
      }
      break;
    case 4:
      {
        Serial.println(" SOUTH_NORTH ");
        event_ID = 8;
        data_available = true;
      }
      break;
    case 5:
      {
        Serial.println(" NORTH_SOUTH ");
        event_ID = 7;
        data_available = true;
      }
      break;
      //  default:
      //    { Serial.println(" not a jesture ");} break;

      //  autohome.sendPacket( packet.c_str() );
  }
}

void touch(unsigned char type) {

  if (lastTouch == type) return;

  // Serial.println("Got touched >.< ");
  // Serial.print(type,DEC);
  switch (type) {
    case 0:
      {
        Serial.println(" --DOUBLETAP_CENTER ");
        event_ID = 21;
        data_available = true;
      }
      break;
    case 1:
      {
        Serial.println(" --DOUBLETAP_EAST ");
        event_ID = 25;
        data_available = true;
      }
      break;
    case 2:
      {
        Serial.println(" --DOUBLETAP_NORTH ");
        event_ID = 24;
        data_available = true;
      }
      break;
    case 3:
      {
        Serial.println(" --DOUBLETAP_WEST ");
        event_ID = 23;
        data_available = true;
      }
      break;
    case 4:
      {
        Serial.println(" --DOUBLETAP_SOUTH ");
        event_ID = 22;
        data_available = true;
      }
      break;
    case 5:
      {
        Serial.println(" SW_TAP_CENTER ");
        event_ID = 11;
        data_available = true;
      }
      break;
    case 6:
      {
        Serial.println(" SW_TAP_EAST ");
        event_ID = 15;
        data_available = true;
      }
      break;
    case 7:
      {
        Serial.println(" SW_TAP_NORTH ");
        event_ID = 14;
        data_available = true;
      }
      break;
    case 8:
      {
        Serial.println(" SW_TAP_WEST ");
        event_ID = 13;
        data_available = true;
      }
      break;
    case 9:
      {
        Serial.println(" SW_TAP_SOUTH ");
        event_ID = 12;
        data_available = true;
      }
      break;
    case 10:
      {
        Serial.println(" SW_TOUCH_CENTER ");
        event_ID = 16;
        data_available = true;
      }
      break;
    case 11:
      {
        Serial.println(" SW_TOUCH_EAST ");
        event_ID = 20;
        data_available = true;
      }
      break;
    case 12:
      {
        Serial.println(" SW_TOUCH_NORTH ");
        event_ID = 19;
        data_available = true;
      }
      break;
    case 13:
      {
        Serial.println(" SW_TOUCH_WEST ");
        event_ID = 18;
        data_available = true;
      }
      break;
    case 14:
      {
        Serial.println(" SW_TOUCH_SOUTH ");
        event_ID = 17;
        data_available = true;
      }
      break;
      //  default:
      //    { Serial.println(" errororo");} break;
  }

  lastTouch = type;
}

void airwheel(int delta) {
  //  Serial.println("Got airwheel ");
  // Serial.print(delta);

  switch (delta) {
    case -11:
      {
        Serial.println(" clockwise?  ");
        autohome.sendPacket( "clockwise");
      //  event_ID = 6;
      //  data_available = true;
      }
      break;
    case 11:
      {
        Serial.println(" counter clock wise ! ");
        autohome.sendPacket( "anticlockwise");
      //  event_ID = 5;
      //  data_available = true;
      }
      break;
    default:
      {
        Serial.println(" no not. ");
      }
      break;
  }
}
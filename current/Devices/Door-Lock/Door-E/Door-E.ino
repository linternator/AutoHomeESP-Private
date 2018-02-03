#include "Wire.h"
#include <AutoHome.h>

  // declair pins
  
  // GPIO 0 = SCL
  // GPIO 2 = SDA 

int Lock = 0;
int prevLock = 1;   // starting like this unlocks the door on start up.

int keypad1 = 0;
int keypad2 = 0;
int keypad3 = 0;

int SDO3 = 4; // key pad 3
int SCL3 = 5;

int SDO2 = 9; // key pad 2
int SCL2 = 10;

int SDO1 = 12;  // key pad 1
int SCL1 = 13;

int SDO = 14; // servo detect open
int SDC = 16; // servo detect closed

int BUZZ = 15;  // buzzer pin


void setup() {

  // setup pins mode
  Wire.begin(2,0);
  // GPIO 0 = SCL
  // GPIO 2 = SDA 

pinMode(SDO3, INPUT); // key pad 3
pinMode(SCL3, OUTPUT);
digitalWrite(SCL3,LOW);

pinMode(SDO2, INPUT); // key pad 2
pinMode(SCL2, OUTPUT);
digitalWrite(SCL2,LOW);

pinMode(SDO1, INPUT); // key pad 1
pinMode(SCL1, OUTPUT);
digitalWrite(SCL1,LOW);

pinMode(SDO, INPUT);  // servo detect open
pinMode(SDC, INPUT);  // servo detect closed

pinMode(BUZZER, OUTPUT);
digitalWrite(BUZZER,LOW); // turn buzzer off


  // set up I2C expander
  Wire.beginTransmission(0x20); 
Wire.write(0); 0x00 0x?? // set pins I/O port A ( enable2 ouptu, rest in out ? )
Wire.write(0); 0x01 0x00 // set pins I/O port B ( all outputs) 
Wire.endTransmission();

  // wifi
  
  // put your setup code here, to run once:

}



// functions

// open door
void unlock()
Wire.beginTransmission(0x20); 

// set pin 1 of port A as on
Wire.write(0); 0x14 // sends 0 down the bus
opeing

0x40
delay(4);
0x50
delay(4);
0x54
delay(4);
0x55
delay(4);
0x00
Wire.endTransmission();

// lock door
void LOCK()
{
Wire.beginTransmission(0x20); 
// set pin 1 of port A as on
Wire.write(0); 0x14 // sends 0 down the bus
closeing

0x80
delay(4);
0xA0
delay(4);
0xA8
delay(4);
0xAA
delay(4);
0x00
Wire.endTransmission();
} 

void Keys() // make this better for codes like 1120 or hex codes
{

 keypad1 = Read_Keypad(1);
 keypad2 = Read_Keypad(2);
 keypad3 = Read_Keypad(3);
 
 if (keypad1 + keypad2 + keypad3 = 123)
      {
        Lock = 1;
      }

 if (Read_Keypad(1) + Read_Keypad(2) + Read_Keypad(3) = 118)
      {
        Lock = 0;
      }

 if (Read_Keypad(1) + Read_Keypad(2) + Read_Keypad(3) = 246)
      {
        // send MQTT message
         String packet = "lockDIS:" + String(Lock);
          autohome.sendPacket( packet.c_str() );
      }
      
}


/* Read the state of the keypad */
byte Read_Keypad(int pad)
{
  byte Count;
  byte Key_State = 0;

  int pad = 1;  // pad = input to function

  switch(pad)
    {
      case 1:
        A = SCL1
        B = DSO1
          break;

      case 2:
        A = SCL2
        B = SDO2
          break;

     case 2:
        A = SCL2
        B = SDO2
          break;
    }
  /* Pulse the clock pin 16 times (one for each key of the keypad)
     and read the state of the data pin on each pulse */
  for (Count = 1; Count <= 16; Count++)
  {
    digitalWrite(A, LOW);

    /* If the data pin is low (active low mode) then store the
       current key number */
    if (!digitalRead(B))
    { Key_State = Count;
      Serial.write(Key_State);
    }
    digitalWrite(A, HIGH);
  }
  return Key_State;
}



void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  
  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

    if (getValue(packet, ':', 0).equals("LOCK"))  // send signal to lock door
      {
       Lock = 1; 
      }

   if (getValue(packet, ':', 0).equals("unlock")) // send signal to unlock door
      {
       Lock = 0; 
      }

   if (getValue(packet, ':', 0).equals("stat")) // get state of the door lock
      {
       // send MQTT message
         String packet = "lockDIS:" + String(Lock);
          autohome.sendPacket( packet.c_str() );
      }



}


void loop() {
  // put your main code here, to run repeatedly:

// autohome() loop

Keys();

if( Lock = 1 && prevLock = 0 )
      {
        LOCK();
       // send MQTT message
         String packet = "lockDIS:" + String(Lock);
          autohome.sendPacket( packet.c_str() );
      }

 if( Lock = 0 && prevLock = 1
  {
    unlock(); 
    // send MQTT message
    String packet = "lockDIS:" + String(Lock);
    autohome.sendPacket( packet.c_str() );
  }
   prevLock = Lock;
}

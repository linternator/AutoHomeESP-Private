/* FILE:    TTP229_16_Key_Capacitive_Touch_Example
   DATE:    25/02/15
   VERSION: 0.1
   
REVISIONS:

25/02/15 Created version 0.1

This is an example of how to use the Hobby Components 16 key capacitive touch
keypad (HCMODU0079). This example sketch will read the current state of the
of the keypad and if a key is pressed output its key number to the serial port.

The sketch assumes that the keypad is configured to 16 key active low mode
by shorting pads P1-3 and P1-P4 together (see schematic or sport forum for more
information). Connect the keypad to your Arduino as follows:

Keypad......Arduino
VCC.........+5V
GND.........GND
SCL.........Digital pin 8
SDO.........Digital pin 9

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.

This software may not be used directly for the purpose of promoting products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR
LACK OF NEGLIGENCE. HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE
FOR ANY DAMAGES INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER. */



/* Define the digital pins used for the clock and data */
#define SCL_PIN 4
#define SDO_PIN 5


int R = 128;  
int G = 128;
int B = 128;

  int Value  = 0;
  int RValue = 0;
  int GValue = 0;
  int BValue = 0;

int Red = 12; //11; // CORRECT  
int Green = 13;  // CORRECT
int Blue = 14; // CORRECT

 int enable = 2; // 5; // enable for PSU 
//  pinMode(enable,OUTPUT);
//  digitalWrite(enable,HIGH);

/* Used to store the key state */
byte Key;

void setup()
{
  /* Initialise the serial interface */
  Serial.begin(9600);
  /* Configure the clock and data pins */
  pinMode(SCL_PIN, OUTPUT); 
  pinMode(SDO_PIN, INPUT);

  pinMode(Red,OUTPUT);
    pinMode(Green,OUTPUT);
      pinMode(Blue,OUTPUT);

  int enable = 2; // 5; // enable for PSU 
  pinMode(enable,OUTPUT);
  digitalWrite(enable,HIGH);
}


/* Main program */
void loop()
{
  /* Read the current state of the keypad */
  Key = Read_Keypad();
 
  /* If a key has been pressed output it to the serial port */
  if (Key)
    Serial.println(Key);

    delay(100);
    
    switch(Read_Keypad())
    {
      case 0:
      {
     //   digitalWrite(enable,HIGH);
     //   digitalWrite(Red,HIGH);
     //   digitalWrite(Green,HIGH);
     //   digitalWrite(Blue,HIGH);
      }break;

            case 1:
      {
              {
                    if(RValue != 1000)
                              {
                                RValue = RValue + 50;
                                fix();
                              }       
              }
       
      }break;

            case 5:
      {
        if(RValue != 0)
                    {
                                RValue = RValue - 50;
                                fix();
                       } 
      }break;






            case 2:
      {
       //   if(Read_Keypad() == 1)
              {
              //  delay(100);
                    if(GValue != 1000)
                              {
                                GValue = GValue + 50;
                                fix();
                              }       
              }
       
      }break;

            case 6:
      {
        if(GValue != 0)
                    {
                                GValue = GValue - 50;
                                fix();
                       } 
      }break;


            case 3:
      {
       //   if(Read_Keypad() == 1)
              {
              //  delay(100);
                    if(BValue != 1000)
                              {
                                BValue = BValue + 50;
                                fix();
                              }       
              }
       
      }break;

            case 7:
      {
        if(BValue != 0)
                    {
                                BValue = BValue - 50;
                                fix();
                       } 
      }break;
      


            case 4:
      {
       //   if(Read_Keypad() == 1)
              {
              //  delay(100);
                    if(Value != 1000)
                              {

                                Value = Value + 50;
                                
                                RValue = Value;
                                GValue = Value;
                                BValue = Value;
                                fix();
                              }       
              }
       
      }break;

            case 8:
      {
        if(Value != 0)
                    {

                      Value = Value - 50;
                                RValue = Value;
                                GValue = Value;
                                BValue = Value;
                                fix();
                       } 
      }break;
      
    
  }
  /* Wait a little before reading again
     so not to flood the serial port*/
// digitalWrite(enable,LOW);
}

void fix(void)
{
  if( (RValue != 0) || (GValue != 0) || (BValue != 0))
  {
       digitalWrite(enable,LOW);
  }
  else
  {
           digitalWrite(enable,HIGH);
  }
  analogWrite(Red,RValue);
    analogWrite(Green,GValue);
      analogWrite(Blue,BValue);
  Serial.println(RValue);
   Serial.println(GValue);
    Serial.println(BValue);
}

/* Read the state of the keypad */
byte Read_Keypad(void)
{
  byte Count;
  byte Key_State = 0;

  /* Pulse the clock pin 16 times (one for each key of the keypad)
     and read the state of the data pin on each pulse */
  for(Count = 1; Count <= 16; Count++)
  {
    digitalWrite(SCL_PIN, LOW);
   
    /* If the data pin is low (active low mode) then store the
       current key number */
    if (!digitalRead(SDO_PIN))
     { Key_State = Count; } 
           digitalWrite(SCL_PIN, HIGH);
     } 
 
  return Key_State;
}

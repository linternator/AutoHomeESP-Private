int Red = 3;  
int Green = 2;  
int Blue = 1;  
int enable = 5; // enable for PSU  

// int ENable = 23;  // enble / power for buttonz
#define SCL_PIN 7 // switch
#define SDO_PIN 8  // switch

int R = 128;	
int G = 128;
int B = 128;

    int PiN = 0;
  
    int Delly = 0;
    int Speed = 1;
  
  int Value  = 0;
  int RValue = 0;
  int GValue = 0;
  int BValue = 0;
  
  int mode = 0;
  int modeold = 0; // for state change detection.
  
  


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  
  
 pinMode(Red, OUTPUT);
 pinMode(Green, OUTPUT);
 pinMode(Blue, OUTPUT);
 
 pinMode(enable, OUTPUT);
 
/// pinMode(ENable, OUTPUT);


  Serial.begin(9600);
 
// digitalWrite(ENable,HIGH);  // initaly PSU is off
 
 startup();	// turns on psu and flashed all LEDs twice to show its working

// set up interupts  

//set up I2C buttons
//may need more verables for them
}


void loop() {

  
  che();
  
	while(mode == modeold)
		{	// replace with nothing as inteuprs work :3
			che();	
//      serial.Write(mode);
		}

  switch(mode)
      {
         
          case 0:
              {		// mode 0 all off
                digitalWrite(enable,HIGH); // PSU off
//                digitalWrite(ENable,HIGH); // buttons on
               // delay(50); // not needed any more
              } break;
             
          case 1:
              {		// mode 1 white lights
                White();
              } break;
              
          case 2:
              {		// mode 2 Red lights
                r3d();
              } break;
              
         case 3:
              {		// mode 3 Green lights
                gr33n();
              } break;
              
          case 4:
              {		// mode 4 Blue lights
                blu3();
              } break;
              
              case 5:
              {		// mode 5 Rando with fade
                Rando();
              } break;
              
           case 6:
              {		// mode 6 mix like in a club
                mix();
              } break;
              
          case 7:
	     {		// music listen mode
//		music();
	     } break;   


 

	} // end of switch
  
  
che();

modeold = mode;
  
}	// end of main loop, good code! XD 



void startup()   // flash twise to show works
{
        digitalWrite(enable,LOW); 
        analogWrite( Blue, 255 ); 
        delay(250);
        analogWrite( Blue, 0 ); 
        delay(250);
        analogWrite( Blue, 255 ); 
        delay(250);
        analogWrite( Blue, 0 ); 
        digitalWrite(enable,HIGH);
        Serial.println("started");
}



void che() {

  
switch( Read_Keypad())
    {


case 0:
  {
   // mode = 0;
  } break;
   
case 1:
      {
          digitalWrite(enable,HIGH);
          mode = 0;
          
        Serial.println("off");
      } break;
      
case 2:
      {
        mode = 1;
        
        Serial.println("white");
      } break;

case 3:
      {
        mode = 2;
        
        Serial.println("2");
      }break;
      
case 4:
      {
        mode = 3;
        
        Serial.println("3");
      } break;

case 5:
      {
        mode = 4;
      } break;

case 6:
      {
        mode = 5;
      }break;

case 7:
      {
        mode = 6;
      } break;

}
}


 

void White()
{
  digitalWrite(enable,LOW);
  analogWrite( Blue, 255 ); 
  analogWrite( Red, 255 ); 
  analogWrite( Green, 255 ); 
}

void r3d() {
  digitalWrite(enable,LOW);
  analogWrite( Blue, 0 ); 
  analogWrite( Red, 255 ); 
  analogWrite( Green, 0 ); 
}

void gr33n() {
  digitalWrite(enable,LOW);
  analogWrite( Blue, 0 ); 
  analogWrite( Red, 0 ); 
  analogWrite( Green, 255 ); 
}

void blu3() {
  digitalWrite(enable,LOW);
  analogWrite( Blue, 255 ); 
  analogWrite( Red, 0 ); 
  analogWrite( Green, 0 ); 
}
  
  
void Rando() {

// timer inturupts. lots and lots of timer inturupts.

digitalWrite(enable,LOW);
  PiN = random(1,4);
  Value = random(0,255);
  Delly = random(10,125);
  Speed = random(1,5);

 // set inturput 
  
if(PiN == 2)
            {
                if(Value < RValue)
                    {
                      for(int fadeValue = RValue ; fadeValue >= Value; fadeValue -=1) { 
                      analogWrite( Red, fadeValue ); 
                      delay(Delly);
              che();       
            		delay(10); // posibly change ? 
                  } RValue = Value;}
                      else
                            {
                                for(int fadeValue = RValue ; fadeValue <= random(0, 255); fadeValue +=1) { 
                                analogWrite( Red, fadeValue );   
                                delay(Delly);  
                    che();            
                                delay(10);                            
                              }RValue = Value;}
            }
            
            
if(PiN == 3)
            {
                if(Value < GValue)
                  {
                      for(int fadeValue = GValue ; fadeValue >= Value; fadeValue -=1) { 
                      analogWrite( Green, fadeValue ); 
                      delay(Delly); 
       che();               
                      delay(10);                            
                  } GValue = Value; }
                      else
                            {
                                for(int fadeValue = GValue ; fadeValue <= Value; fadeValue +=1) { 
                                analogWrite( Green, fadeValue ); 
                                delay(Delly); 
                 che();               
                                delay(10);                            
                              } GValue = Value; }
            }       
          
if(PiN == 1)
                        {
                if(Value < BValue)
                  {
                      for(int fadeValue = BValue ; fadeValue >= Value; fadeValue -=1) { 
                      analogWrite( Blue, fadeValue ); 
                      delay(Delly); 
       che();               
                      delay(10);                            
                  } BValue = Value; }
                      else
                            {
                                for(int fadeValue = GValue ; fadeValue <= Value; fadeValue +=1) { 
                                analogWrite( Blue, fadeValue ); 
                                delay(Delly);                
                                delay(10);                            
                              } BValue = Value; }
            }
      delay(Delly); 
che();      
  } 
  
  
  
  void mix() {
  Value = random(0,2);
   PiN = random(1,4);
    Delly = random(10,125);
    
  if(PiN == 2)
        {
          if(Value = 0)
              {
                   analogWrite( Red, 0 );
              }
                    else
                          {
                            analogWrite( Red, 255 );
                          }
        }
        
  if(PiN == 1)
        {
          if(Value = 0)
              {
                   analogWrite( Green, 0 );
              }
                    else
                          {
                            analogWrite( Green, 255 );
                          }
        }
        
  if(PiN == 3)
        {
          if(Value = 0)
              {
                   analogWrite( Blue, 0 );
              }
                    else
                          {
                            analogWrite( Blue, 255 );
                          }
        }
        
     //   delay(1000);
        
        
        for(int fadeValue = Delly ; fadeValue >= 0; fadeValue -=1)
             {
               delay(10);
               che();
             }
             
             analogWrite( Blue, 0 );
             analogWrite( Red, 0 );
             analogWrite( Green, 0 );
 
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
      Key_State = Count;
   
    digitalWrite(SCL_PIN, HIGH);
  } 
 
  return Key_State;
}
